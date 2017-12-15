//-----------------------------------------------------------------------------
//
//		Класс для чтения конфигурационных XML-файлов
//		(с) РГУПС, ВЖД 17/09/2016
//		Разработал: Притыкин Д.Е.
//
//-----------------------------------------------------------------------------
/*!
 *  \file
 *  \brief Класс для чтения конфигурационных XML-файлов
 *  \copyright  РГУПС, ВЖД
 *  \author Притыкин Д.Е.
 *  \date 17/09/2016
 */

#include "CfgReader.h"
#include "convert.h"
#include <QTextStream>

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CfgReader::CfgReader()
{

}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
CfgReader::~CfgReader()
{
	
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::load(QString path)
{
	// Пытаемся открыть конфигурационный файл
	file_name = path;
	file = new QFile(file_name);

	if (!file->open(QFile::ReadOnly | QFile::Text))
    {
		return false;
	}

	// Читаем содержимое файла
	domDoc.setContent(file);

	// Закрываем файл
	file->close();

	// Читаем корневой элемент
	firstElement = domDoc.documentElement();

	// Проверяем имя корневого элемента
	if (firstElement.tagName() != "Config")
    {
		return false;
	}	
	
	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getString(QString section,
                          QString field,
                          QString &value)
{
	QDomNode secNode = getFirstSection(section);

	if (secNode.isNull())
    {
		return false;
    }

	QDomNode fieldNode = getField(secNode, field);

	if (fieldNode.isNull())
    {
		return false;
	}

	value = fieldNode.toElement().text();

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getDouble(QString section, QString field, double &value)
{
	QString tmp;
    if (!getString(section, field, tmp))
	{
		return false;
	}

    if (!TextToDouble(tmp, value))
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getInt(QString section, QString field, int &value)
{
	QString tmp;
    if (!getString(section, field, tmp))
	{
		return false;
	}

    if (!TextToInt(tmp, value))
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
QDomNode CfgReader::getFirstSection(QString section)
{
	QDomNode node = firstElement.firstChild();

	while ( (node.nodeName() != section) && (!node.isNull()) )
	{
		node = node.nextSibling();
	}

	curNode = node;

	return node;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
QDomNode CfgReader::getNextSection()
{
	QString section = curNode.nodeName();
	QDomNode node = curNode.nextSibling();

	while ((node.nodeName() != section) && (!node.isNull()))
	{
		node = node.nextSibling();
	}

	curNode = node;

	return node;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
QDomNode CfgReader::getField(QDomNode secNode, QString field)
{
	QDomNode node = secNode.firstChild();

	while ((node.nodeName() != field) && (!node.isNull()))
	{
		node = node.nextSibling();
	}

	return node;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getString(QDomNode secNode, QString field, QString &value)
{
	QDomNode node = getField(secNode, field);

	if (node.isNull())
    {
		return false;
	}

	value = node.toElement().text();

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getDouble(QDomNode secNode, QString field, double &value)
{
	QString tmp;

    if (!getString(secNode, field, tmp))
	{
		return false;
	}

    if (!TextToDouble(tmp, value))
	{
		return false;
	}

	return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getInt(QDomNode secNode, QString field, int &value)
{
	QString tmp;

    if (!getString(secNode, field, tmp))
	{
		return false;
	}

    if (!TextToInt(tmp, value))
	{
		return false;
	}

    return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getBool(QString section, QString field, bool &value)
{
    QString tmp;
    if (!getString(section, field, tmp))
    {
        return false;
    }

    tmp = EraseSpaces(tmp);

    if ( (tmp == "True") || (tmp == "true") || (tmp == "1") )
        value = true;
    else
    {
        if ( (tmp == "False") || (tmp == "false") || (tmp == "0") )
            value = false;
        else
            return false;
    }

    return true;
}

//-----------------------------------------------------------------------------
//
//-----------------------------------------------------------------------------
bool CfgReader::getBool(QDomNode secNode, QString field, bool &value)
{
    QString tmp;

    if (!getString(secNode, field, tmp))
    {
        return false;
    }

    tmp = EraseSpaces(tmp);

    if ( (tmp == "True") || (tmp == "true") || (tmp == "1") )
        value = true;
    else
    {
        if ( (tmp == "False") || (tmp == "false") || (tmp == "0") )
            value = false;
        else
            return false;
    }

    return true;
}
