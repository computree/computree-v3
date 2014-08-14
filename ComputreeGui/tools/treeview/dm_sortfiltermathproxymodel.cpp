#include "dm_sortfiltermathproxymodel.h"

#include "dm_guimanager.h"

#include <QLocale>

DM_SortFilterMathProxyModel::DM_SortFilterMathProxyModel(QObject *parent) :
    QSortFilterProxyModel(parent)
{
    m_parser.SetDecSep(QLocale::system().decimalPoint().toLatin1());
}

void DM_SortFilterMathProxyModel::setVariableInMathExpression(const QString &var)
{
    m_var = var;
}

bool DM_SortFilterMathProxyModel::setMathExpression(const QString &expression)
{
    if(m_var.isEmpty())
        return false;

    if(expression.isEmpty())
    {
        m_mathExpression = expression;
        invalidateFilter();
        return true;
    }

    if(!expression.contains(m_var))
        return false;

    QString tmp = expression;
    tmp.replace(m_var, "1");

    mu::string_type expression_buffer;
    QByteArray expression_array = tmp.toLocal8Bit();
    char *expression_chars = expression_array.data();
    expression_buffer = expression_chars;

    try
    {
        // test expression
        m_parser.SetExpr(expression_buffer);
        m_parser.Eval();
    }
    catch(mu::Parser::exception_type &e)
    {
        GUI_LOG->addMessage(LogInterface::debug, LogInterface::gui, tr("Erreur dans l'expression mathématique : ") + QString::fromStdString(e.GetMsg()));
        return false;
    }

    m_mathExpression = expression;
    m_mathExpression.replace('.', QLocale::system().decimalPoint());
    m_mathExpression.replace(',', QLocale::system().decimalPoint());

    invalidateFilter();

    return true;
}

QString DM_SortFilterMathProxyModel::variableInMathExpression() const
{
    return m_var;
}

bool DM_SortFilterMathProxyModel::filterAcceptsRow(int source_row, const QModelIndex &source_parent) const
{
    if(m_mathExpression.isEmpty())
        return QSortFilterProxyModel::filterAcceptsRow(source_row, source_parent);

    QModelIndex source_index = sourceModel()->index(source_row, filterKeyColumn(), source_parent);

    if (!source_index.isValid())
        return true;

    QString key = sourceModel()->data(source_index, filterRole()).toString();

    QString tmp = m_mathExpression;
    tmp.replace(m_var, key);

    mu::string_type expression_buffer;
    QByteArray expression_array = tmp.toLocal8Bit();
    char *expression_chars = expression_array.data();
    expression_buffer = expression_chars;

    try
    {
        // test expression
        m_parser.SetExpr(expression_buffer);
        return m_parser.Eval() != 0;
    }
    catch(mu::Parser::exception_type &e)
    {
        return false;
    }

    return true;
}
