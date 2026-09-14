#pragma once

#include <QByteArray>
#include <QHash>
#include <QObject>
#include <QReadWriteLock>
#include <QString>

namespace pad {

class MediaStorage : public QObject {
  Q_OBJECT

  QHash<QString, QByteArray> _mediaCache;
  mutable QReadWriteLock _mediaCacheLock;

public:
  MediaStorage(QObject *parent = nullptr);
  QByteArray getMedia(const QString &path) const;
  void addMedia(const QString &path, const QByteArray &data);
  QString addMediaFromSystem(const QString &systemPath);
};

} // namespace pad
