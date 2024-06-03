#pragma once

#include "cseries/cseries.hpp"

const wchar_t k_xml_tag_start_character = '<';

template<typename t_character_type>
struct c_xml_tag_parser
{
public:
	virtual bool parse_tag(t_character_type const* a1, long* a2, t_character_type* a3, long a4, long* a5) = 0;
	virtual ~c_xml_tag_parser() { }

	c_xml_tag_parser(t_character_type const* xml_tag) :
		m_xml_tag(xml_tag)
	{
		DECLFUNC(0x014E8230, c_xml_tag_parser<t_character_type>*, __thiscall, c_xml_tag_parser<t_character_type>*, t_character_type const*)(this, xml_tag);
	}

protected:
	t_character_type const* m_xml_tag;
	//wchar_t m_magic_character;
};
static_assert(sizeof(c_xml_tag_parser<wchar_t>) == 0x8);
//static_assert(sizeof(c_xml_tag_parser<wchar_t>) == 0xC);

