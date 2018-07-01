#ifndef SITE_PARSERS_H_
#define SITE_PARSERS_H_

#include "stdafx.h"

namespace l2clientextender
{
	namespace parsers
	{
		class AbstractParser
		{
		protected:
			std::string siteUrl;

			std::string downloadPage();
			std::string preprocesPage(std::string &rawPage);
		public:
			AbstractParser(const std::string url);
			std::string getSiteURL();
			virtual std::string getVotePageContent(std::string &pageHtml) = 0;
		};

		class HopzoneParser final : public AbstractParser
		{
		public:
			HopzoneParser(const std::string url);
			std::string getVotePageContent(std::string &pageHtml);
		};

		class Lineage2Top200Parser final : public AbstractParser
		{
		public:
			Lineage2Top200Parser(const std::string url);
			std::string getVotePageContent(std::string &pageHtml);
		};
	}
}

#endif