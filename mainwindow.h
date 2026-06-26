#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QTableWidget>
#include <QPushButton>
#include <QLineEdit>
#include <QCheckBox>
#include <QRadioButton>
#include <QStackedWidget>
#include <QLabel>
#include <QProgressBar>
#include <QMovie>
#include <QPropertyAnimation>
#include <QSet>
#include <QHash>
#include <QColor>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QStringList>
#include <QMouseEvent>
#include <QPaintEvent>

// ==================== CONSTANTS ====================

#define FROZEN_COLUMN_WIDTH_DATE 90
#define FROZEN_COLUMN_WIDTH_TIME 80
#define DEFAULT_COLUMN_WIDTH 100
#define MAX_ROWS 2000000
#define MAX_VISIBLE_ROWS 100000
#define CHUNK_SIZE_LOAD 5000

// ==================== LOGIN WINDOW ====================

class LoginWindow : public QDialog
{
    Q_OBJECT
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    QString loggedInRole() const;
    void updateCredentials(const QString& adminUser, const QString& adminPass,
                           const QString& regUser, const QString& regPass);

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void checkLogin();
    void togglePasswordVisibility();
    void animateError();
    void onEditCredentials();

private:
    void loadSavedCredentials();
    void saveCredentialsToFile();
    bool validateCredentials(const QString& adminUser, const QString& adminPass,
                             const QString& userUser, const QString& userPass);

    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor& color) { m_backgroundColor = color; }

    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *loginButton;
    QPushButton *togglePasswordBtn;
    QLabel *errorLabel;
    QLabel *infoLabel;
    QPropertyAnimation *errorAnimation;

    QColor m_backgroundColor;
    bool m_dragging;
    QPoint m_dragPosition;
    QString m_loggedInRole;

    QString m_adminUsername;
    QString m_adminPassword;
    QString m_userUsername;
    QString m_userPassword;

    QLineEdit *tempAdminUserEdit;
    QLineEdit *tempAdminPassEdit;
    QLineEdit *tempUserUserEdit;
    QLineEdit *tempUserPassEdit;
};

// ==================== MAIN WINDOW ====================

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void changeDefaultCredentials(const QString& newAdminUser, const QString& newAdminPass,
                                  const QString& newUserUser, const QString& newUserPass);

private slots:
    void browseFile();
    void loadFile();
    void exportCSV();
    void findText();
    void clearSearch();
    void clearRowColumnHighlights();
    void clearAllFilters();
    void saveSettings();
    void resetSettings();
    void applyTheme(bool checked);
    void toggleHighlighting(bool enabled);
    void changeRowHighlightColor();
    void changeColumnHighlightColor();
    void onTableCellClicked(int row, int column);
    void onFrozenTableCellClicked(int row, int column);
    void onHeaderClicked(int column);
    void showLoginDialog();
    void updateAccessRights();
    void switchToPage(int index);
    void onChangeDefaultCredentials();
    void handleVirtualScroll(int value);
    void loadMoreRows();

private:
    // UI Setup
    void setupUI();
    void setupTopToolbar(QVBoxLayout *mainLayout);
    void setupMenuButtons(QVBoxLayout *mainLayout);
    void setupMainContent(QVBoxLayout *mainLayout);
    void setupDataPage();
    void setupFrozenTable();
    void setupDataTable();
    void setupSettingsPage();
    void setupAdminGroup(QVBoxLayout *settingsLayout);
    void setupUserGroup(QVBoxLayout *settingsLayout);
    void setupThemeGroup(QVBoxLayout *settingsLayout);
    void setupHighlightingGroup(QVBoxLayout *settingsLayout);
    void setupSettingsButtons(QVBoxLayout *settingsLayout);
    void setupConnections();
    void setupLoadingOverlay();
    void loadDefaultFile();
    void setupEmptyTable();
    void applyInitialTheme();
    void showColumnFilterMenu(int column);
    void applyColumnFilter(int column, const QStringList& selectedValues);
    void clearColumnFilter(int column);
    void updateFilterIndicator(int column, bool hasFilter);
    void rebuildFilterMaps();
    void clearTables();
    void addFrozenRow(int row, const QString& date, const QString& time);
    QTableWidgetItem* createTableItem(const QString& text);
    void syncRowHeights();
    void syncFrozenTableAfterSort();
    void refreshFrozenTableVisibility();
    void applyCrossHighlight(int row, int column);
    void clearSearchHighlights();
    void showLoadingOverlay(const QString& message);
    void hideLoadingOverlay();
    void updateLoadingProgress(int percent, const QString& message);
    QPushButton* createButton(const QString& text, int height, const QSize& fixedSize = QSize());
    QGroupBox* createGroupBox(const QString& title);
    QString fixTimeFormat(const QString& time);
    void updateColorButtons();
    void loadSettings();
    bool saveSettingsInternal();
    void applyLightTheme();
    void applyDarkTheme();

    // Large file handling methods
    bool hasSufficientMemory();
    void freeMemory();
    void setupVirtualScrolling();

    // UI Components
    QTableWidget *frozenTableWidget;
    QTableWidget *dataTableWidget;
    QPushButton *pushButtonBrowse;
    QPushButton *exportButton;
    QPushButton *loginButton;
    QPushButton *findButton;
    QPushButton *clearSearchButton;
    QPushButton *m_clearHighlightsButton;
    QPushButton *m_clearFiltersButton;
    QPushButton *m_resetSettingsButton;
    QPushButton *btnDataView;
    QPushButton *btnSettings;
    QPushButton *saveSettingsButton;
    QPushButton *rowHighlightColorButton;
    QPushButton *columnHighlightColorButton;
    QLineEdit *searchEdit;
    QLineEdit *adminUsernameEdit;
    QLineEdit *adminPasswordEdit;
    QLineEdit *userUsernameEdit;
    QLineEdit *userPasswordEdit;
    QCheckBox *enableHighlightingCheckbox;
    QRadioButton *lightThemeRadio;
    QRadioButton *darkThemeRadio;
    QStackedWidget *stackedWidget;

    // Loading overlay
    QWidget *m_loadingOverlay;
    QLabel *m_loadingLabel;
    QLabel *m_loadingProgressLabel;
    QProgressBar *m_loadingProgressBar;
    QMovie *m_loadingMovie;
    bool m_loadingActive;

    // Data
    QString currentFile;
    QStringList headers;
    int m_maxRowsLoaded;
    int m_currentHighlightedRow;
    int m_currentHighlightedColumn;
    QColor m_rowHighlightColor;
    QColor m_columnHighlightColor;
    bool m_highlightingEnabled;
    QString m_currentRole;
    QString m_currentTheme;
    QString m_adminUsername;
    QString m_adminPassword;
    QString m_userUsername;
    QString m_userPassword;

    // Filtering
    QHash<int, QStringList> m_columnFilters;
    QHash<int, QSet<QString>> m_columnUniqueValues;
    QSet<QTableWidgetItem*> m_searchHighlightedItems;

    // Large file handling
    QStringList m_fullFileData;
    int m_totalRowsInFile;
    int m_currentPage;
    bool m_fileFullyLoaded;
    QHash<int, QStringList> m_rowCache;
    int m_currentCacheStartRow;
    int m_currentCacheEndRow;
    int m_cacheStartRow;
    int m_cacheEndRow;
    QHash<int, QStringList> m_cachedData;
};

#endif // MAINWINDOW_H