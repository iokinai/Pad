#include <padimageprovider.hpp>

namespace pad {

PadImageProvider::PadImageProvider(MediaStorage *storage)
    : QQuickImageProvider(QQuickImageProvider::Image), _storage(storage) {}

QImage PadImageProvider::requestImage(const QString &id, QSize *size,
                                      const QSize &requestedSize) {
  QByteArray image = _storage->getMedia(id);
  if (image.size() == 0) {
    return QImage();
  }

  QImage img = QImage::fromData(image);

  if (img.isNull()) {
    return QImage();
  }

  if (requestedSize.width() > 0 && requestedSize.height() > 0) {
    return img.scaled(requestedSize, Qt::KeepAspectRatio,
                      Qt::SmoothTransformation);
  }

  return img;
}

} // namespace pad
