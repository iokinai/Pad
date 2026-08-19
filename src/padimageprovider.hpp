#pragma once

#include "storage/mediastorage.hpp"
#include <QQuickImageProvider>

namespace pad {

class PadImageProvider : public QQuickImageProvider {
  MediaStorage *_storage;

public:
  PadImageProvider(MediaStorage *storage);

  QImage requestImage(const QString &id, QSize *size,
                      const QSize &requestedSize) override;
};

} // namespace pad
