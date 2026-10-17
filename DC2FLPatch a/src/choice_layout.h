#pragma once

// Proportional choice labels only. No dialogue or confirmation font settings.
#include <windows.h>
#include <string>
#include <vector>

namespace ChoiceLayout {
constexpr int kHeight = 28;
constexpr int kTextLeft = 32;
constexpr int kInset = 16;

inline bool IsSpace(wchar_t ch) {
  return ch == L' ' || ch == L'\t' || ch == 0x3000 || ch == 0xa0;
}

// The caller passes a bounded, decrypted CP932 label. Normalize full-width
// Latin and word spaces after recognizing the choice marker; never match prose.
inline bool Decode(const std::string &encoded, std::wstring &body) {
  if (encoded.empty() || encoded.size() > 1024)
    return false;
  int count = MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, encoded.data(),
                                  (int)encoded.size(), nullptr, 0);
  if (count <= 0)
    return false;
  std::wstring wide(count, L'\0');
  if (!MultiByteToWideChar(932, MB_ERR_INVALID_CHARS, encoded.data(),
                           (int)encoded.size(), &wide[0], count))
    return false;
  size_t start = 0;
  while (start < wide.size() && IsSpace(wide[start]))
    ++start;
  if (start == wide.size() || wide[start++] != 0x25cb)
    return false;
  body.clear();
  bool pendingSpace = false;
  for (; start < wide.size(); ++start) {
    wchar_t ch = wide[start];
    if (ch >= 0xff01 && ch <= 0xff5e)
      ch -= 0xfee0;
    // Old MES imports used ~ for a space to avoid encrypted NUL bytes.
    if (IsSpace(ch) || ch == L'~') {
      pendingSpace = !body.empty();
      continue;
    }
    if (ch < 0x20 || ch == 0x7f || ch == L'@' ||
        (ch >= 0x3040 && ch <= 0x30ff) || (ch >= 0x3400 && ch <= 0x9fff))
      return false;
    if (pendingSpace)
      body.push_back(L' ');
    pendingSpace = false;
    body.push_back(ch);
  }
  return !body.empty();
}

struct Bitmap {
  int width = 0;
  int fontSize = 0;
  std::vector<BYTE> pixels;
};

using ExtentFn = BOOL(WINAPI *)(HDC, LPCWSTR, int, LPSIZE);
using GlyphFn = DWORD(WINAPI *)(HDC, UINT, UINT, LPGLYPHMETRICS, DWORD, LPVOID, const MAT2 *);

// Draw complete runs using real GDI functions, outside the game's per-glyph
// spacing hooks. Alpha 0..4 is the native 28x28 blitter's format.
inline bool Raster(HDC dc, const std::wstring &text, int left, int width,
                   std::vector<BYTE> &pixels, ExtentFn extentFn, GlyphFn glyphFn) {
  TEXTMETRICW metrics = {};
  if (!GetTextMetricsW(dc, &metrics))
    return false;
  const int baseline = (kHeight - metrics.tmHeight) / 2 + metrics.tmAscent;
  MAT2 matrix = {};
  matrix.eM11.value = matrix.eM22.value = 1;
  for (size_t i = 0; i < text.size(); ++i) {
    SIZE prefix = {};
    if (i && !extentFn(dc, text.c_str(), (int)i, &prefix))
      return false;
    GLYPHMETRICS gm = {};
    DWORD bytes = glyphFn(dc, text[i], GGO_GRAY8_BITMAP, &gm, 0, nullptr, &matrix);
    if (bytes == GDI_ERROR || bytes > 65536)
      return false;
    if (!bytes)
      continue;
    std::vector<BYTE> glyph(bytes);
    if (glyphFn(dc, text[i], GGO_GRAY8_BITMAP, &gm, bytes, glyph.data(), &matrix) == GDI_ERROR)
      return false;
    DWORD stride = (gm.gmBlackBoxX + 3) & ~3u;
    if ((ULONGLONG)stride * gm.gmBlackBoxY > bytes)
      return false;
    for (DWORD gy = 0; gy < gm.gmBlackBoxY; ++gy) {
      for (DWORD gx = 0; gx < gm.gmBlackBoxX; ++gx) {
        BYTE alpha = (BYTE)((glyph[gy * stride + gx] + 8) / 16);
        if (!alpha)
          continue;
        int x = left + prefix.cx + gm.gmptGlyphOrigin.x + (int)gx;
        int y = baseline - gm.gmptGlyphOrigin.y + (int)gy;
        if (x < 0 || x >= width || y < 0 || y >= kHeight)
          return false;
        BYTE &pixel = pixels[y * width + x];
        if (alpha > pixel)
          pixel = alpha;
      }
    }
  }
  return true;
}

inline HFONT MakeFont(int size) {
  return CreateFontW(-size, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE,
                     ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
                     ANTIALIASED_QUALITY, DEFAULT_PITCH, L"Arial");
}

inline void RasterMarker(int width, std::vector<BYTE> &pixels) {
  // Arial's white-circle character is a small bullet. Keep the original menu's
  // roughly 22px ring and a stable 10px gap before the proportional label.
  for (int y = 2; y < 26; ++y) {
    for (int x = 0; x < 24; ++x) {
      int covered = 0;
      for (int sy = 0; sy < 4; ++sy) {
        for (int sx = 0; sx < 4; ++sx) {
          const double dx = x + (sx + 0.5) / 4.0 - 12.0;
          const double dy = y + (sy + 0.5) / 4.0 - 14.0;
          const double distance2 = dx * dx + dy * dy;
          if (distance2 >= 9.0 * 9.0 && distance2 <= 10.75 * 10.75)
            ++covered;
        }
      }
      pixels[y * width + x] = (BYTE)((covered + 2) / 4);
    }
  }
}

inline bool Build(const std::wstring &body, int maxWidth, Bitmap &bitmap,
                  ExtentFn extentFn, GlyphFn glyphFn) {
  if (body.empty() || maxWidth < 64 || maxWidth > 768)
    return false;
  HDC dc = CreateCompatibleDC(nullptr);
  if (!dc)
    return false;
  bool success = false;
  // The circle and text start stay aligned across options of different lengths.
  for (int size = 28; size >= 18 && !success; --size) {
    HFONT font = MakeFont(size);
    if (!font)
      break;
    HGDIOBJ old = SelectObject(dc, font);
    SIZE extent = {};
    bool fits = extentFn(dc, body.c_str(), (int)body.size(), &extent) &&
                kTextLeft + extent.cx + 2 <= maxWidth;
    std::vector<BYTE> pixels(maxWidth * kHeight, 0);
    if (fits)
      fits = Raster(dc, body, kTextLeft, maxWidth, pixels, extentFn, glyphFn);
    SelectObject(dc, old);
    DeleteObject(font);
    if (!fits)
      continue;
    RasterMarker(maxWidth, pixels);
    int right = 0;
    for (int y = 0; y < kHeight; ++y)
      for (int x = 0; x < maxWidth; ++x)
        if (pixels[y * maxWidth + x])
          right = max(right, x + 1);
    bitmap.width = right;
    bitmap.fontSize = size;
    bitmap.pixels.resize(right * kHeight);
    for (int y = 0; y < kHeight; ++y)
      memcpy(bitmap.pixels.data() + y * right, pixels.data() + y * maxWidth, right);
    success = right > kTextLeft;
  }
  DeleteDC(dc);
  return success;
}
} // namespace ChoiceLayout
