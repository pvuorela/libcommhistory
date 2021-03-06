/******************************************************************************
**
** This file is part of libcommhistory.
**
** Copyright (C) 2014-2015 Jolla Ltd.
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** Contact: Reto Zingg <reto.zingg@jolla.com>
**
** This library is free software; you can redistribute it and/or modify it
** under the terms of the GNU Lesser General Public License version 2.1 as
** published by the Free Software Foundation.
**
** This library is distributed in the hope that it will be useful, but
** WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
** or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
** License for more details.
**
** You should have received a copy of the GNU Lesser General Public License
** along with this library; if not, write to the Free Software Foundation, Inc.,
** 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
**
******************************************************************************/

#ifndef COMMHISTORY_SINGLE_EVENT_MODEL_H
#define COMMHISTORY_SINGLE_EVENT_MODEL_H

#include "eventmodel.h"
#include "libcommhistoryexport.h"

namespace CommHistory {

class SingleEventModelPrivate;

/*!
 * \class SingleEventModel
 * \brief Model representing single event
 * e.g. phone number or IM user id
 */
class LIBCOMMHISTORY_EXPORT SingleEventModel : public EventModel
{
    Q_OBJECT

public:
    /*!
     * Model constructor.
     *
     * \param parent Parent object.
     */
    explicit SingleEventModel(QObject *parent = 0);

    /*!
     * Destructor.
     */
    ~SingleEventModel();

    /*!
     * Polulate model with existing event.
     *
     * \param id, event id to be fetched from database
     *
     * \return true if successful, otherwise false
     */
    bool getEventById(int eventId);

    /*!
     * Populate model with existing event identified by message token or mms id.
     *
     * \param token, message token or empty string
     * \param mmsId, mms id or empty string
     * \param groupId, valid group id or -1 to ignore
     *
     * \return true if successful, otherwise false
     */
    bool getEventByTokens(const QString &token,
                          const QString &mmsId,
                          int groupId);

    /**
     * Returns the event fetched by one of the above functions.
     */
    Event event() const;

private:
    Q_DECLARE_PRIVATE(SingleEventModel)
};

} // namespace CommHistory

#endif // SINGLEEVENTMODEL_H
