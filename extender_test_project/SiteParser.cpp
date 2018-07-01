#include "stdafx.h"

using namespace std;
using namespace l2clientextender;
using namespace l2clientextender::parsers;

// ==============================================================
// ==============================================================

AbstractParser::AbstractParser(const string url) :siteUrl(url)
{
}

string AbstractParser::getSiteURL()
{
	return siteUrl;
}

string AbstractParser::downloadPage()
{
	return "";
}

string AbstractParser::preprocesPage(string &rawPage)
{
	return "";
}

// ==============================================================
// ==============================================================

HopzoneParser::HopzoneParser(const string url):AbstractParser(url)
{
}

string HopzoneParser::getVotePageContent(string &pageHtml)
{
	return "";
}

// ==============================================================
// ==============================================================

Lineage2Top200Parser::Lineage2Top200Parser(const string url) :AbstractParser(url)
{
}

string Lineage2Top200Parser::getVotePageContent(string &pageHtml)
{
	return "";
}