/*  taglib_handler.cc - this file is part of MediaTomb.

    Copyright (C) 2005 Gena Batyan <bgeradz@deadlock.dhs.org>,
    Sergey Bostandzhyan <jin@deadlock.dhs.org>

    MediaTomb is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    MediaTomb is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with MediaTomb; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/// \file taglib_handler.cc
/// \brief Implementeation of the TagHandler class.

#ifdef HAVE_CONFIG_H
    #include "autoconfig.h"
#endif

#ifdef HAVE_TAGLIB

#include <taglib.h>
#include <fileref.h>
#include <tag.h>
#include <audioproperties.h>
#include <tstring.h>

#include "taglib_handler.h"
#include "string_converter.h"
#include "common.h"
#include "tools.h"

using namespace zmm;

TagHandler::TagHandler() : MetadataHandler()
{
}
       
static void addField(metadata_fields_t field, TagLib::FileRef *fileTag, Ref<CdsItem> item)
{
    TagLib::String val;
    String value;
    unsigned int i;
    
    TagLib::Tag *tag = fileTag->tag();
    
    if (tag == NULL) 
        return;

    if (tag->isEmpty())
        return;

    Ref<StringConverter> sc = StringConverter::m2i();
    
    switch (field)
    {
        case M_TITLE:
            val = tag->title();
            break;
        case M_ARTIST:
            val = tag->artist();
            break;
        case M_ALBUM:
            val = tag->album();
            break;
        case M_DATE:
            i = tag->year();
            if (i > 0)
            {
                value = String::from(i);

                if (string_ok(value))
                    value = value + _("-00-00");
            }
            else
                return;
            break;
        case M_GENRE:
            val = tag->genre();
            break;
        case M_DESCRIPTION:
            val = tag->comment();
            break;
        case M_TRACKNUMBER:
            i = tag->track();
            if (i > 0)
            {
                value = String::from(i);
            }
            else
                return;
            break;
        default:
            return;
    }

    if ((field != M_DATE) && (field != M_TRACKNUMBER))
        value = String((char *)val.toCString());

    value = trim_string(value);
    
    if (string_ok(value))
    {
        item->setMetadata(String(MT_KEYS[field].upnp), sc->convert(value));
//        log_debug("Setting metadata on item: %d, %s\n", field, sc->convert(value).c_str());
    }
}

void TagHandler::fillMetadata(Ref<CdsItem> item)
{
    log_debug("adding metadata for: %s\n", item->getLocation().c_str());
    
    TagLib::FileRef tag(item->getLocation().c_str());

    if (tag.isNull())
        return;

    for (int i = 0; i < M_MAX; i++)
        addField((metadata_fields_t) i, &tag, item);
    
    int temp;
    
    TagLib::AudioProperties *audioProps = tag.audioProperties();
    
    if (audioProps == NULL) 
        return;
    
    // note: UPnP requres bytes/second
    temp = audioProps->bitrate() * 1024 / 8;

    if (temp > 0)
    {
        item->getResource(0)->addAttribute(MetadataHandler::getResAttrName(R_BITRATE),
                                           String::from(temp)); 
    }

    temp = audioProps->length();
    if (temp > 0)
    {
        item->getResource(0)->addAttribute(MetadataHandler::getResAttrName(R_DURATION),
                                           secondsToHMS(temp));
    }

    temp = audioProps->sampleRate();
    if (temp > 0)
    {
        item->getResource(0)->addAttribute(MetadataHandler::getResAttrName(R_SAMPLEFREQUENCY), 
                                           String::from(temp));
    }

    temp = audioProps->channels();

    if (temp > 0)
    {
        item->getResource(0)->addAttribute(MetadataHandler::getResAttrName(R_NRAUDIOCHANNELS),
                                           String::from(temp));
    }
    
}

Ref<IOHandler> TagHandler::serveContent(Ref<CdsItem> item, int resNum)
{
    return nil;
}

#endif // HAVE_TAGLIB
