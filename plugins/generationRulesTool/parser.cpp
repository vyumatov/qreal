#include "parser.h"

#include "ast/callGeneratorFor.h"
#include "ast/complexIdentifier.h"
#include "ast/foreach.h"
#include "ast/identifier.h"
#include "ast/incomingLinks.h"
#include "ast/links.h"
#include "ast/newline.h"
#include "ast/node.h"
#include "ast/outcomingLinks.h"
#include "ast/program.h"
#include "ast/text.h"

using namespace simpleParser;

using namespace qrtext::core;

simpleParser::Parser::Parser(QList<qrtext::core::Error> &errors)
	: qrtext::core::Parser<TokenTypes>(grammar(), errors)
{
}

QSharedPointer<qrtext::core::ParserInterface<TokenTypes>> simpleParser::Parser::grammar()
{
	ParserRef<TokenTypes> statement;

	auto program = *statement
			>> [] (QSharedPointer<TemporaryList> statementList) {
				QList<QSharedPointer<ast::Node>> result;
				for (auto const &stat : statementList->list()) {
					if (stat->is<TemporaryList>()) {
					// it is a list of assignments
						for (auto const &assignment : qrtext::as<TemporaryList>(stat)->list()) {
							result << assignment;
						}
					} else {
						// It is expression.
						result << stat;
					}
				}

				if (result.size() == 1) {
					// Do not create Program node for simple expression.
					return result.first();
				} else {
					return qrtext::wrap(new ast::Program(result));
				}
	};

	auto identifier = TokenTypes::identifier
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Identifier(token.lexeme());
	};

	auto complexIdentifier = (identifier & -TokenTypes::dot & identifier)
			>> [] (QSharedPointer<TemporaryPair> tokens) {
				auto identifier = tokens->left();
				auto property = tokens->right();

				return qrtext::wrap(new ast::ComplexIdentifier(identifier, property));
	};

	auto outcomingLinksIdentifier = (identifier & -TokenTypes::dot & -TokenTypes::outcomingLinksKeyword)
			>> [] (QSharedPointer<ast::Node> identifierNode) {
				return qrtext::wrap(new ast::OutcomingLinks(identifierNode));
	};

	auto incomingLinksIdentifier = (identifier & -TokenTypes::dot & -TokenTypes::incomingLinksKeyword)
			>> [] (QSharedPointer<ast::Node> identifierNode) {
				return qrtext::wrap(new ast::IncomingLinks(identifierNode));
	};

	auto linksIdenitifer = (identifier & -TokenTypes::dot & -TokenTypes::linksKeyword)
			>> [] (QSharedPointer<ast::Node> identifierNode) {
				return qrtext::wrap(new ast::Links(identifierNode));
	};

	auto foreachStatement = (-TokenTypes::foreachKeyword & -TokenTypes::openingBracket & identifier
				& -TokenTypes::inKeyword
				& (identifier | outcomingLinksIdentifier | incomingLinksIdentifier | linksIdenitifer)
				& -TokenTypes::closingBracket
				& -TokenTypes::openingCurlyBracket & program & -TokenTypes::closingCurlyBracket)
			>> [] (QSharedPointer<TemporaryPair> statementPair) {
				auto identifierAndType = qrtext::as<TemporaryPair>(statementPair->left());
				auto identifier = identifierAndType->left();
				auto type = identifierAndType->right();

				auto program = statementPair->right();

				return qrtext::wrap(new ast::Foreach(identifier, type, program));
	};

	auto callGeneratorForStatement = (-TokenTypes::callGeneratorForKeyword & -TokenTypes::openingBracket
				& identifier & -TokenTypes::closingBracket)
			>> [] (QSharedPointer<ast::Node> identifierNode) {
				return qrtext::wrap(new ast::CallGeneratorFor(identifierNode));
	};

	auto text = TokenTypes::text
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Text(token.lexeme());
	};

	auto newline = TokenTypes::newlineKeyword
			>> [] (Token<TokenTypes> const &token) {
				return new ast::Newline();
	};

	statement = text | newline | complexIdentifier | foreachStatement | callGeneratorForStatement;

	return program.parser();
}
