/* ------------------------------------------------------------------
overlayhelp.cpp is part of picture-show.
.....................................................................

picture-show is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

picture-show is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with picture-show. If not, see <http://www.gnu.org/licenses/>.

......................................................................

author: Sebastian Sch�fer
March 2012

--------------------------------------------------------------------*/

#include "overlayhelp.h"

#include <iostream>

overlayHelp::overlayHelp(QWidget *pWidget) : overlayBase(pWidget)
{
    this->language = "de";
    this->mouseControl = false;
}

void overlayHelp::setLanguage(const QString & lang)
{
    this->language = lang;
}

void overlayHelp::setMouseControl(bool mc)
{
    this->mouseControl = mc;
}

void overlayHelp::generatePixmap()
{
    if (this->m_background.isNull())
        return;

    this->m_overlay = QPixmap(this->m_paintWidget->width(), this->m_paintWidget->height());
    this->m_overlay.fill(QColor(0, 0, 0, 0));

    int rectWidth;
    int rectHeight;
    int rectPosX;
    int rectPosY;

    double opacity = 0.9;

    if (this->m_paintWidget->width() > this->m_paintWidget->height())
    {
        rectHeight = int(this->m_paintWidget->height() * 0.9);
        rectWidth = rectHeight;
        rectPosX = int((this->m_paintWidget->width() - rectWidth) / 2.0);
        rectPosY = int((this->m_paintWidget->height() - rectHeight) / 2.0);
    }
    else
    {
        rectWidth = rectHeight = int(this->m_paintWidget->width() * 0.9);
        rectHeight = rectWidth;
        rectPosX = int((this->m_paintWidget->width() - rectWidth) / 2.0);
        rectPosY = int((this->m_paintWidget->height() - rectHeight) / 2.0);
    }

    int borderthickness = int(rectHeight * 0.04);

    int rect2Width = rectWidth - borderthickness;
    int rect2Height = rectHeight - borderthickness;
    int rect2PosX = rectPosX + int(borderthickness / 2.0);
    int rect2PosY = rectPosY + int(borderthickness / 2.0);

    int skipH = int(rect2Width * 0.05);
    int skipV = int(rect2Height * 0.05);

    QBrush rectBrush(QColor::fromRgb(255, 255, 255, 180));
    QPen rectPen(QColor::fromRgb(255, 255, 255, 0));
    rectPen.setWidth(0);

    QLinearGradient linearGrad(0.0, 0.0, this->m_paintWidget->width(), this->m_paintWidget->height());
    linearGrad.setColorAt(0, QColor::fromRgb(0, 0, 0, 150));
    linearGrad.setColorAt(1, QColor::fromRgb(0, 0, 0, 240));
    QBrush rect2Brush(linearGrad);

    QPen rect2Pen(QColor::fromRgb(0, 0, 0, 0));
    rect2Pen.setWidth(0);

    QPixmap logo(":/images/img/logo256x256.png");
    logo = logo.scaledToHeight(int(rect2Height*0.15), Qt::SmoothTransformation);

    QPixmap logoText(":/images/img/logoText.png");
    logoText = logoText.scaledToHeight(int(rect2Height*0.05), Qt::SmoothTransformation);

    QString lang_fn = ":/images/img/commands_";
    if (this->mouseControl)
        lang_fn += "mouse_";
    else
        lang_fn += "keyboard_";

    QPixmap commands(lang_fn + this->language + ".png");
    if (commands.width() <= 0)
        commands = QPixmap(lang_fn + "de");

    commands = commands.scaledToWidth(int(rect2Width-skipH), Qt::SmoothTransformation);

    QString version = tr("version: %1").arg(qApp->applicationVersion());
    QFont fontVersion(QString("Helvetica"), int(rectHeight*0.015));
    QFontMetrics fm(fontVersion);
    int widthVersion = fm.width(version);
    int heightVersion = fm.height();

    QRect rectVersion((int((this->m_paintWidget->width() / 2.0) - (widthVersion / 2.0))),
                      rect2PosY + 6 + logo.height() + logoText.height(),
                      widthVersion, heightVersion);


    QFont fontHeader(QString("Helvetica"), int(rectWidth * 0.022), QFont::Bold);
    fontHeader.setStyleStrategy(QFont::PreferAntialias);
    QFontMetrics fmHeader(fontHeader);
    QPixmap headerLogo(":/images/img/headerLogo.png");
    headerLogo = headerLogo.scaledToHeight(int(fmHeader.height()*0.9), Qt::SmoothTransformation);
    QRect rectHeader(rect2PosX + int(headerLogo.width()*1.5) + int(skipH/2.0), rect2PosY + (logo.height() + logoText.height() + heightVersion + int(skipV/2.0)), rect2Width - int(headerLogo.width()*1.5) - skipH, fmHeader.height());
    QString header = tr("Kommandos");

    int appropiateSize = this->calcTextSize(rect2Width - (skipH*4.5));
    QFont fontText1(QString("Helvetica"), appropiateSize, QFont::Bold);
    QFont fontText2(QString("Helvetica"), appropiateSize);
    fontText1.setStyleStrategy(QFont::PreferAntialias);
    fontText2.setStyleStrategy(QFont::PreferAntialias);
    QFontMetrics fmText(fontText2);
    QRect rectText1(rect2PosX + int(skipH/2.0),  rect2PosY + (logo.height() + logoText.height() + heightVersion + fmHeader.height() + skipV), skipH*3, fmText.height());
    QRect rectText2(rect2PosX + int(skipH*3.5),  rect2PosY + (logo.height() + logoText.height() + heightVersion + fmHeader.height() + skipV), rect2Width - (skipH*4), fmText.height());
    QRect rectCommands(rect2PosX + int(skipH/2.0), rect2PosY + (logo.height() + logoText.height() + heightVersion + skipV),int(rect2Width-skipH), commands.height());

    QStringList items = this->helpItems();

    QPixmap header2Logo(":/images/img/header2Logo.png");
    header2Logo = header2Logo.scaledToHeight(int(fmHeader.height()*0.9), Qt::SmoothTransformation);
    QRect rectHeader2(rect2PosX + int(header2Logo.width()*1.5) + int(skipH/2.0), rect2PosY + rect2Width - fmHeader.height() - (4.0*fmText.height()) - skipV, rect2Width - int(header2Logo.width()*1.5) - skipH, fmHeader.height());
    QString header2 = tr("�ber picture-show");

    QRect rectAbout(rect2PosX + int(skipH/2.0),  rect2PosY + rect2Width - (fmText.height()*4.0) - int(skipV/2.0), rect2Width - skipH, (fmText.height() * 4.0) + skipV);
    QString about = tr("Dieses Programm unterliegt der GPL v3 Lizenz. Es kann frei verwendet und ver�ndert werden, muss allerdings stets unter GPL Lizenz ver�ffentlicht werden. Es verwendet das Qt Framework (version %1). \n\n2012 by Sebastian Sch�fer / web: http://code.google.com/p/picture-show/").arg(QT_VERSION_STR);

    QPainter p;
    p.begin(&this->m_overlay);
    p.setRenderHint(QPainter::Antialiasing);
    p.setRenderHint(QPainter::HighQualityAntialiasing);
    p.setRenderHint(QPainter::TextAntialiasing);
    p.setOpacity(opacity);
    p.setPen(rectPen);
    p.setBrush(rectBrush);
    p.drawRoundedRect(rectPosX, rectPosY, rectWidth, rectHeight, 22.0, 22.0);
    p.setPen(rect2Pen);
    p.setBrush(rect2Brush);
    p.drawRoundedRect(rect2PosX, rect2PosY, rect2Width, rect2Height, 20.0, 20.0);
    p.drawPixmap(int((this->m_paintWidget->width() / 2.0) - (logo.width() / 2.0)), rect2PosY + 6, logo.width(), logo.height(), logo);
    p.drawPixmap(int((this->m_paintWidget->width() / 2.0) - (logoText.width() / 2.0)), rect2PosY + 6 + logo.height(), logoText.width(), logoText.height(), logoText);
    p.setPen(QPen(Qt::white));
    p.setFont(fontVersion);
    p.drawText(rectVersion, Qt::AlignHCenter, version);
    //p.drawPixmap(rect2PosX + int(skipH/2.0), rect2PosY + (logo.height() + logoText.height() + heightVersion + int(skipV/2.0)), headerLogo.width(), headerLogo.height(), headerLogo);
    p.setFont(fontHeader);
    //p.drawText(rectHeader, Qt::AlignLeft, header);

//    for (int i = 0; i < items.size(); i+=2)
//    {
//        p.setFont(fontText1);
//        p.drawText(rectText1, Qt::AlignLeft, items.at(i));
//        p.setFont(fontText2);
//        p.drawText(rectText2, Qt::AlignLeft, items.at(i+1));
//        rectText1.moveTop(rectText1.top() + int(fmText.height() * 1.3));
//        rectText2.moveTop(rectText2.top() + int(fmText.height() * 1.3));

//        if (rectText1.bottom() > rect2PosY + rect2Width - fmHeader.height() - (2.0*fmText.height()) - skipV)
//            break;
//    }

//    p.drawRect(rectCommands);
    p.setOpacity(0.98);
    p.drawPixmap(rectCommands, commands);

    p.setOpacity(opacity);
    p.drawPixmap(rectHeader2.x() - int(header2Logo.width()*1.5), rectHeader2.y(), header2Logo.width(), header2Logo.height(), header2Logo);

    p.setFont(fontHeader);
    p.drawText(rectHeader2, Qt::AlignLeft, header2);
    p.setFont(fontText2);
    p.drawText(rectAbout, Qt::AlignLeft | Qt::TextWordWrap, about);

    p.end();
}

QStringList overlayHelp::helpItems()
{
    QStringList items;
    items << "O" << tr("�ffnet den Dialog zur Auswahl das Bilderordners und der Einstellungen f�r die picture show");
    items << "F" << tr("Wechselt zwischen Fullscreen- und Fenstermodus hin und her");
    items << "" << tr("");
    items << tr("<Pfeil rechts>") << tr("Zum n�chsten Bild blenden");
    items << tr("<Maus rechts>") << tr("Zum n�chsten Bild blenden (wenn aktiviert)");
    items << tr("<Pfeil links>") << tr("Zum vorherigen Bild blenden");
    items << tr("<Maus links>") << tr("Zum vorherigen Bild blenden (wenn aktiviert)");
    items << tr("<Bild ab>") << tr("Springt zum n�chsten Bild");
    items << tr("<Bild auf>") << tr("Springt zum vorherigen Bild");
    items << "J" << tr("Jump-to Dialog zum springen zu einem Bild, Eingabe der Bildnummer gefolgt von <Enter>");
    items << tr("I") << tr("Anzeige mit Infos �ber das aktuelle Bild anzeigen");
    items << tr("<Leertaste>") << tr("Startet den Timer, Eingabe der Dauer in Sekunden (4-99) gefolgt von <Enter>");
    items << "" << tr("Beendet den Timer, wenn aktiv");
    items << "" << tr("");
    items << "H / F1" << tr("Diese Hilfe anzeigen");
    items << "ESC" << tr("Blendet Info-Leise oder diese Hilfe aus bzw. beendet das Programm");

    return items;
}

int overlayHelp::calcTextSize(int targetSize)
{
    QStringList entries = this->helpItems();

    int size = 35;
    QFont testFont(QString("Helvetica"), size);
    bool sizeAppropriate = false;

    while (!sizeAppropriate)
    {
        sizeAppropriate = true;
        size--;
        testFont.setPointSize(size);

        for (int i = 1; i < entries.size(); i += 2)
        {
            QFontMetrics fm(testFont);
            int widthVersion = fm.width(entries.at(i));
            if (widthVersion > targetSize)
            {
                sizeAppropriate = false;
                break;
            }
        }
    }

    return size;
}
