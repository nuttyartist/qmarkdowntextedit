/*
 * Copyright (c) 2014-2019 Patrizio Bekerle -- http://www.bekerle.com
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; version 2 of the License.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License
 * for more details.
 *
 * QPlainTextEdit markdown highlighter
 */


#pragma once

#include <QTextCharFormat>
#include <QSyntaxHighlighter>
#include <QRegularExpression>

QT_BEGIN_NAMESPACE
class QTextDocument;

QT_END_NAMESPACE

class MarkdownHighlighter : public QSyntaxHighlighter
{
Q_OBJECT

public:
    enum HighlightingOption{
        None = 0,
        FullyHighlightedBlockQuote = 0x01
    };
    Q_DECLARE_FLAGS(HighlightingOptions, HighlightingOption)

    MarkdownHighlighter(QTextDocument *parent = nullptr,
                        HighlightingOptions highlightingOptions =
                        HighlightingOption::None);

    // we use some predefined numbers here to be compatible with
    // the peg-markdown parser
    enum HighlighterState {
        NoState = -1,
        Link = 0,
        Image = 3,
        CodeBlock,
        Italic = 7,
        Bold,
        List,
        Comment = 11,
        H1,
        H2,
        H3,
        H4,
        H5,
        H6,
        BlockQuote,
        HorizontalRuler = 21,
        Table,
        InlineCodeBlock,
        MaskedSyntax,
        CurrentLineBackgroundColor,
        BrokenLink,
        FrontmatterBlock,
        TrailingSpace,

        //code highlighting
        CodeKeyWord = 1000,
        CodeString = 1001,
        CodeComment = 1002,
        CodeType = 1003,
        CodeOther = 1004,
        CodeNumLiteral = 1005,
        CodeBuiltIn = 1006,

        // internal
        CodeBlockEnd = 100,
        HeadlineEnd,
        FrontmatterBlockEnd,

        //languages
        CodeCpp = 200,
        CodeJs = 201,
        CodeC = 202,
        CodeBash = 203,
        CodePHP = 204,
        CodeQML = 205,
        CodePython = 206,
        CodeRust = 207,
        CodeJava = 208,
        CodeCSharp = 209,
        CodeGo = 210,
        CodeV = 211,
        CodeSQL = 212,
        CodeJSON = 213,
        CodeXML = 214
    };
    Q_ENUMS(HighlighterState)

//    enum BlockState {
//        NoBlockState = 0,
//        H1,
//        H2,
//        H3,
//        Table,
//        CodeBlock,
//        CodeBlockEnd
//    };

    void setTextFormats(QHash<HighlighterState, QTextCharFormat> formats);
    void setTextFormat(HighlighterState state, QTextCharFormat format);
    void clearDirtyBlocks();
    void setHighlightingOptions(const HighlightingOptions options);
    void initHighlightingRules();

signals:
    void highlightingFinished();

protected slots:
    void timerTick();

protected:
    struct HighlightingRule {
        explicit HighlightingRule(const HighlighterState state_) : state(state_) {}
        HighlightingRule() = default;

        QRegularExpression pattern;
        HighlighterState state = NoState;
        int capturingGroup = 0;
        int maskedGroup = 0;
        bool useStateAsCurrentBlockState = false;
        bool disableIfCurrentStateIsSet = false;
    };

    void highlightBlock(const QString &text) Q_DECL_OVERRIDE;

    void initTextFormats(int defaultFontSize = 12);

    void highlightMarkdown(const QString& text);

    void highlightHeadline(const QString& text);

    void highlightAdditionalRules(const QVector<HighlightingRule> &rules,
                                  const QString& text);

    void highlightCodeBlock(const QString& text);

    void highlightSyntax(const QString &text);

    void highlightFrontmatterBlock(const QString& text);

    void highlightCommentBlock(QString text);

    void addDirtyBlock(const QTextBlock& block);

    void reHighlightDirtyBlocks();

    void setHeadingStyles(const QTextCharFormat &format,
                     const QRegularExpressionMatch &match,
                     const int capturedGroup);

    static void initCodeLangs();

    void xmlHighlighter(const QString &text);

    QVector<HighlightingRule> _highlightingRulesPre;
    QVector<HighlightingRule> _highlightingRulesAfter;
    static QHash<QString, HighlighterState> langStringToEnum;
    QVector<QTextBlock> _dirtyTextBlocks;
    QHash<HighlighterState, QTextCharFormat> _formats;
    QTimer *_timer;
    bool _highlightingFinished;
    HighlightingOptions _highlightingOptions;

    void setCurrentBlockMargin(HighlighterState state);
};
