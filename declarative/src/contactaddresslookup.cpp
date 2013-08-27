/* Copyright (C) 2013 Jolla Ltd.
 * Contact: John Brooks <john.brooks@jollamobile.com>
 *
 * You may use this file under the terms of the BSD license as follows:
 *
 * "Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in
 *     the documentation and/or other materials provided with the
 *     distribution.
 *   * Neither the name of Nemo Mobile nor the names of its contributors
 *     may be used to endorse or promote products derived from this
 *     software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
 */

#include "contactaddresslookup.h"

#include <QTimer>

using namespace CommHistory;

ContactAddressLookup::ContactAddressLookup(QObject *parent)
    : QObject(parent), mContactId(0), requestPending(false)
{
    listener = ContactListener::instance();
    connect(listener.data(), SIGNAL(contactUpdated(quint32,QString,QList<ContactAddress>)),
            SLOT(onContactUpdated(quint32,QString,QList<ContactAddress>)));
    connect(listener.data(), SIGNAL(contactRemoved(quint32)),
            SLOT(onContactRemoved(quint32)));
}

void ContactAddressLookup::setLocalUid(const QString &uid)
{
    if (uid == mLocalUid)
        return;

    mLocalUid = uid;

    if (mContactId != 0) {
        mContactId = 0;
        emit contactIdChanged();
    }

    emit localUidChanged();

    if (!requestPending) {
        QTimer::singleShot(0, this, SLOT(request()));
        requestPending = true;
    }
}

void ContactAddressLookup::setRemoteUid(const QString &uid)
{
    if (uid == mRemoteUid)
        return;

    mRemoteUid = uid;

    if (mContactId != 0) {
        mContactId = 0;
        emit contactIdChanged();
    }

    emit remoteUidChanged();

    if (!requestPending) {
        QTimer::singleShot(0, this, SLOT(request()));
        requestPending = true;
    }
}

void ContactAddressLookup::request()
{
    requestPending = false;

    if (mLocalUid.isEmpty() || mRemoteUid.isEmpty())
        return;

    listener->resolveContact(mLocalUid, mRemoteUid);
}

void ContactAddressLookup::onContactUpdated(quint32 id, const QString &name, const QList<ContactAddress> &addresses)
{
    Q_UNUSED(name);

    if (ContactListener::addressMatchesList(mLocalUid, mRemoteUid, addresses)) {
        if (mContactId != int(id)) {
            mContactId = int(id);
            emit contactIdChanged();
        }
    }

    emit contactUpdated(int(id));
}

void ContactAddressLookup::onContactRemoved(quint32 id)
{
    if (mContactId && mContactId == int(id)) {
        mContactId = 0;
        emit contactIdChanged();
    }

    emit contactRemoved(int(id));
}

