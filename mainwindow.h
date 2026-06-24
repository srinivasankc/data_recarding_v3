#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QStackedWidget>
#include <QPropertyAnimation>
#include <QColor>
#include <QSet>
#include <QGroupBox>
#include <QRadioButton>
#include <QCheckBox>
#include <QProgressBar>
#include <QMenu>
#include <QAction>
#include <QHeaderView>
#include <QScrollBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTimer>
#include <QStatusBar>
#include <QApplication>
#include <QScreen>
#include <QDebug>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QEasingCurve>
#include <QGraphicsDropShadowEffect>
#include <QStyle>
#include <QRegularExpression>
#include <QColorDialog>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>

// Constants
const int MAX_ROWS = 50000;
const int CHUNK_SIZE = 1000;
const int FROZEN_COLUMN_WIDTH_DATE = 100;
const int FROZEN_COLUMN_WIDTH_TIME = 100;
const int DEFAULT_COLUMN_WIDTH = 100;

// ==================== LOGIN WINDOW CLASS ====================
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
    void onEditCredentials();

private:
    bool validateCredentials(const QString& adminUser, const QString& adminPass,
                             const QString& userUser, const QString& userPass);
    void animateError();
    void loadSavedCredentials();
    void saveCredentialsToFile();
    QColor backgroundColor() const { return m_backgroundColor; }
    void setBackgroundColor(const QColor &color) { m_backgroundColor = color; update(); }

    QLabel *logoLabel;
    QLabel *titleLabel;
    QLabel *subtitleLabel;
    QLineEdit *usernameEdit;
    QLineEdit *passwordEdit;
    QPushButton *togglePasswordBtn;
    QPushButton *loginButton;
    QLabel *errorLabel;
    QLabel *infoLabel;
    QPropertyAnimation *errorAnimation;
    QColor m_backgroundColor;
    bool m_dragging;
    QPoint m_dragPosition;
    QString m_loggedInRole;

    // Default credentials - can be changed by user
    QString m_adminUsername;
    QString m_adminPassword;
    QString m_userUsername;
    QString m_userPassword;

    // Temporary edit fields
    QLineEdit *tempAdminUserEdit;
    QLineEdit *tempAdminPassEdit;
    QLineEdit *tempUserUserEdit;
    QLineEdit *tempUserPassEdit;
};

// ==================== MAIN WINDOW CLASS ====================
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
    void exportCSV();
    void findText();
    void clearSearch();
    void clearRowColumnHighlights();
    void onTableCellClicked(int row, int column);
    void onFrozenTableCellClicked(int row, int column);
    void toggleHighlighting(bool enabled);
    void changeRowHighlightColor();
    void changeColumnHighlightColor();
    void onHeaderClicked(int column);
    void showLoginDialog();
    void saveSettings();
    void resetSettings();
    void applyTheme(bool checked);
    void switchToPage(int index);
    void onChangeDefaultCredentials();

private:
    void setupUI();
    void setupTopToolbar(QVBoxLayout *mainLayout);
    void setupMenuButtons(QVBoxLayout *mainLayout);
    void setupMainContent(QVBoxLayout *mainLayout);
    void setupDataPage();
    void setupSettingsPage();
    void setupFrozenTable();
    void setupDataTable();
    void setupAdminGroup(QVBoxLayout *settingsLayout);
    void setupThemeGroup(QVBoxLayout *settingsLayout);
    void setupHighlightingGroup(QVBoxLayout *settingsLayout);
    void setupSettingsButtons(QVBoxLayout *settingsLayout);
    void setupConnections();
    void setupLoadingOverlay();
    void loadFile();
    void loadDefaultFile();
    void setupEmptyTable();
    void clearTables();
    void addFrozenRow(int row, const QString& date, const QString& time);
    QTableWidgetItem* createTableItem(const QString& text);
    void syncRowHeights();
    void syncFrozenTableAfterSort();
    void refreshFrozenTableVisibility();
    void applyCrossHighlight(int row, int column);
    void clearSearchHighlights();
    void applyLightTheme();
    void applyDarkTheme();
    void applyInitialTheme();
    void updateAccessRights();
    void loadSettings();
    bool saveSettingsInternal();
    void updateColorButtons();
    void showColumnFilterMenu(int column);
    void applyColumnFilter(int column, const QStringList& selectedValues);
    void clearColumnFilter(int column);
    void clearAllFilters();
    void updateFilterIndicator(int column, bool hasFilter);
    void rebuildFilterMaps();
    void showLoadingOverlay(const QString& message);
    void hideLoadingOverlay();
    void updateLoadingProgress(int percent, const QString& message);
    QString fixTimeFormat(const QString& time);

    QPushButton* createButton(const QString& text, int height, const QSize& fixedSize = QSize());
    QGroupBox* createGroupBox(const QString& title);

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
    QCheckBox *enableHighlightingCheckbox;
    QRadioButton *lightThemeRadio;
    QRadioButton *darkThemeRadio;
    QStackedWidget *stackedWidget;
    QWidget *m_loadingOverlay;
    QLabel *m_loadingLabel;
    QLabel *m_loadingProgressLabel;
    QProgressBar *m_loadingProgressBar;
    QMovie *m_loadingMovie;
    bool m_loadingActive;

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

    QString currentFile;
    QStringList headers;
    QMap<int, QStringList> m_columnFilters;
    QMap<int, QSet<QString>> m_columnUniqueValues;
    QVector<QVector<QString>> m_cachedData;
    int m_currentCacheStartRow;
    int m_currentCacheEndRow;
    QSet<QTableWidgetItem*> m_searchHighlightedItems;
};

#endif // MAINWINDOW_H