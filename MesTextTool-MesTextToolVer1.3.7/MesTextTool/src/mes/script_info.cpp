#include <iostream>
#include <optional>
#include <algorithm> 
#include <ranges>
#include <mes.hpp>
#include <xstr.hpp>

namespace mes
{
	std::vector<script_info> script_info::script_infos
	{
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
	};

	[[maybe_unused]] static auto __script_infos_init__ = []()
	{
		const auto infos{ const_cast<std::vector<script_info>*>(&script_info::infos()) };
		std::ranges::sort(infos->begin(), infos->end(), std::greater{}, &script_info::version);
		return true;
	}();

	auto script_info::infos() noexcept -> const std::vector<script_info>& 
	{
		return script_info::script_infos;
	}

	auto script_info::parse(std::string_view data) noexcept -> const script_info*
	{
		const auto parts{ xstr::view{ data }.split(',') };

		if (parts.empty() || parts.size() < 14)
		{
			return nullptr;
		}

		std::string_view name{ xstr::trim(parts[0]) };
		if (name.empty())
		{
			return nullptr;
		}

		script_info::offset_t offset_type_value{};
		std::string_view offset_type{ xstr::trim(parts[1]) };
		if (offset_type.size() != 7 && !offset_type.starts_with("offset"))
		{
			return nullptr;
		}
		else if (offset_type[6] == '1')
		{
			offset_type_value = script_info::offset_t::offset1;
		}
		else if (offset_type[6] == '2')
		{
			offset_type_value = script_info::offset_t::offset2;
		}
		else 
		{
			return nullptr;
		}

		std::optional version{ xstr::to_integer<uint16_t>(xstr::trim(parts[2]), 16) };
		if (!version.has_value())
		{
			return nullptr;
		}

		uint8_t values[11]{};
		for (size_t i{ 0 }; i < sizeof(values); i++)
		{
			const std::string_view part{ xstr::trim(parts[i + 3]) };
			if(part.empty())
			{
				return nullptr;
			}

			std::optional value{ xstr::to_integer<uint8_t>(part, 16)};
			if (!value.has_value())
			{
				return nullptr;
			}

			values[i] = value.value();
		}

		std::vector<uint8_t> opstrs{};
		if (parts.size() > 14)
		{
			for (size_t i{ 14 }; i < parts.size(); i++)
			{
				std::optional value{ xstr::to_integer<uint8_t>(parts[i], 16) };
				if (!value.has_value())
				{
					return nullptr;
				}
				opstrs.push_back(value.value());
			}
		}

		script_info newinfo
		{
			.name    = std::string{ name },
			.offset  = offset_type_value,
			.version = version.value(),
			.opstrs  = std::move(opstrs)
		};

		std::memcpy((void*)(&newinfo.uint8x2), values, sizeof(values));
		
		bool is_existing{ false };
		for (auto& info : script_info::script_infos)
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
			script_info::script_infos.push_back(script_info{});
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
			script_info& back{ script_info::script_infos.back() };
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
			script_info::script_infos.begin(),
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
			script_info::script_infos.end(),
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
		for (const auto& info : script_info::script_infos)
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
			};
		}

		return nullptr;
	}

	auto script_info::query(const std::string_view name) noexcept -> const script_info*
	{
		if (!name.empty())
		{
			for (const auto& info : script_info::script_infos)
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
		for (const auto& info : script_info::script_infos)
		{ "dc2fl"     , offset1, 0x9C69, { 0x00, 0x28 }, { 0x29, 0x2E }, { 0x2F, 0x49 }, { 0x4A, 0x4D }, { 0x4E, 0xFF },  0x20, { 0x35, 0x45 }/*---*/}, // D.C.II Fall in Love ～ダ・カーポII～フォーリンラブ
