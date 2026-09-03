#pragma once

#include <QCoro/QCoroQmlTask>
#include <QCoro/QCoroTask>
#include <QObject>
#include <QStandardPaths>
#include <notes/notesmodel.hpp>
#include <pages/editor/note.hpp>
#include <search/searchcontroller.hpp>
#include <storage/notescache.hpp>

namespace pad {

class NotesController : public QObject {
  Q_OBJECT

  static constexpr const int LOAD_PACKET_LENGTH = 10;

  size_t _loaded = 0;
  size_t _totalCount = 0;
  NotesModel _notesModel;
  QHash<QString, Note *> _notePathMap;
  QSet<Note *> _unsavedNotes;
  NotesCache *_cache;
  SearchController *_searchController;
  bool _loadingNotes = false;
  QString _searchQuery;

  Q_PROPERTY(size_t notesCount READ notesCount NOTIFY notesCountChanged)
  Q_PROPERTY(size_t loadedNotesCount READ loadedNotesCount NOTIFY
                 loadedNotesCountChanged)
  Q_PROPERTY(NotesModel *notes READ notes CONSTANT)
  Q_PROPERTY(bool loadingNotes READ loadingNotes NOTIFY loadingNotesChanged)
  Q_PROPERTY(QString searchQuery READ searchQuery WRITE setSearchQuery NOTIFY
                 searchQueryChanged)

  QVector<QPair<QString, QByteArray>>
  loadMediaWithDataFromStorage(const QStringList &mediaPaths);

  void prepareAndPushCreatedNote(Note *note, bool front = false);

  void setLoadingNotes(bool v) noexcept;

public:
  NotesController(NotesCache *cache, SearchController *searchController,
                  QObject *parent = nullptr);
  size_t notesCount() const;
  NotesModel *notes();
  Q_INVOKABLE void addEmptyNote();
  QCoro::Task<void> saveNoteAsync(Note *note);
  QCoro::Task<void> loadNotesAsync();

  Q_INVOKABLE QCoro::QmlTask qmlSaveNoteAsync(Note *note);
  Q_INVOKABLE QCoro::QmlTask qmlLoadNotesAsync();
  Q_INVOKABLE QString loadImageFromSystem(const QString &systemPath);
  bool loadingNotes() const noexcept;
  size_t loadedNotesCount() const noexcept;

  const QString &searchQuery() const noexcept;
  void setSearchQuery(const QString &newQuery);

signals:
  void notesCountChanged();
  void couldNotSaveNote();
  void loadingNotesChanged();
  void loadedNotesCountChanged();
  void addImageError(const QString &path);
  void searchQueryChanged();

private slots:
  void onNoteAdded();
  void onNoteEdited();
  void onFullNotesLoaded(QVector<Note *> notes);
  void onTotalCountChanged();
  void onNoteCreated(Note *note);
  void onSearchEnded(QVector<LoadedNote> notes);
  void onSearchQueryChanged();
};

} // namespace pad
