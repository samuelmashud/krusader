/*
    SPDX-FileCopyrightText: 2016 Rafi Yanai <krusader@users.sf.net>
    SPDX-FileCopyrightText: 2016 Shie Erlich <krusader@users.sf.net>
    SPDX-FileCopyrightText: 2016-2022 Krusader Krew <https://krusader.org>

    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "krdebuglogger.h"
#include "compat.h"

#include <QStringBuilder>

KrDebugLogger krDebugLogger;

KrDebugLogger::KrDebugLogger()
{
    // Sets the level of detail/verbosity
    const QByteArray krDebugBrief = qgetenv("KRDEBUG_BRIEF").toLower();
    briefMode = (krDebugBrief == "true" || krDebugBrief == "yes" || krDebugBrief == "on" || krDebugBrief == "1");
}

QString KrDebugLogger::indentationEtc(const QString &argFunction, int line, const QString &fnStartOrEnd) const
{
    QString result = QString(indentation - 1, ' ') %  // Applies the indentation level to make logs clearer
                     fnStartOrEnd % argFunction;  // Uses QStringBuilder to concatenate
    if (!briefMode)
        result = QString("Pid:%1 ").arg(getpid()) %
                result %
                (line != 0 ? QString("(%1)").arg(line) : "");
    return result;
}

void KrDebugLogger::decreaseIndentation()
{
    indentation -= indentationIncrease;
}

void KrDebugLogger::increaseIndentation()
{
    indentation += indentationIncrease;
}

// ---------------------------------------------------------------------------------------
// Member functions of the KrDebugFnLogger class
// ---------------------------------------------------------------------------------------

KrDebugFnLogger::KrDebugFnLogger(const QString &argFunction, int line, KrDebugLogger &argKrDebugLogger) :
    functionName(argFunction), krDebugLogger(argKrDebugLogger)
{
    // Shows that a function has been started
    qDebug().nospace().noquote() << krDebugLogger.indentationEtc(functionName, line, "┏");

    krDebugLogger.increaseIndentation();
}

KrDebugFnLogger::~KrDebugFnLogger()
{
    krDebugLogger.decreaseIndentation();
    // Shows that a function is going to finish
    qDebug().nospace().noquote() << krDebugLogger.indentationEtc(functionName, 0, "┗");
}
