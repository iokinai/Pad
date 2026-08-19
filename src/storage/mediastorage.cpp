#include <storage/mediastorage.hpp>
#include <utils.hpp>

#include <QByteArray>
#include <QFile>
#include <QUrl>

namespace pad {

MediaStorage::MediaStorage(QObject *parent) : QObject(parent) {}

QByteArray MediaStorage::getMedia(const QString &path) const {
  QReadLocker locker(&_mediaCacheLock);

  if (!_mediaCache.contains(path)) {
    return QByteArray();
  }
  return _mediaCache.value(path);
}

void MediaStorage::addMedia(const QString &path, const QByteArray &data) {
  QWriteLocker locker(&_mediaCacheLock);
  _mediaCache[path] = data;
}

QString MediaStorage::addMediaFromSystem(const QString &systemPath) {
  QString localPath = QUrl(systemPath).isLocalFile()
                          ? QUrl(systemPath).toLocalFile()
                          : systemPath;
  QFile file(localPath);
  if (!file.open(QIODevice::ReadOnly)) {

    return QString();
  }

  QByteArray data = file.readAll();
  file.close();
  QString path = utils::randomUuid();
  addMedia(path, data);
  return path;
}

} // namespace pad
