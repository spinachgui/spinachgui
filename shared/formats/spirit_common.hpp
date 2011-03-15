
#ifndef SPIRIT_COMMON_H
#define SPIRIT_COMMON_H

#define BOOST_SYSTEM_NO_DEPRECATED

#include <boost/spirit/include/support_istream_iterator.hpp>
#include <boost/spirit/include/qi.hpp>
#include <boost/spirit/version.hpp>
#include <boost/spirit/include/phoenix_core.hpp>
#include <boost/spirit/include/phoenix_operator.hpp>

struct element_tmp {
	element_tmp(int atomic) : atomic_number(atomic) {}
	element_tmp(int atomic,int mass) : atomic_number(atomic),mass_number(mass) {}
	int atomic_number;
	int mass_number;
};


template <typename Expr, typename Iterator = boost::spirit::unused_type>
struct attribute_of_parser
{
	typedef typename boost::spirit::result_of::compile<
		boost::spirit::qi::domain, Expr
		>::type parser_expression_type;
 
	typedef typename boost::spirit::traits::attribute_of<
		parser_expression_type, boost::spirit::unused_type, Iterator
		>::type type;
};

template <typename T>
void display_attribute_of_parser(T const&)
{
	typedef typename attribute_of_parser<T>::type attribute_type;
	std::cout << typeid(attribute_type).name() << std::endl;
}


#endif
