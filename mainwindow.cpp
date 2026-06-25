#include <QGuiApplication>
#include <QStandardPaths>
#include <QDir>
#include "mainwindow.h"
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QWidget>
#include <QFileDialog>
#include <QFile>
#include <QHeaderView>
#include <QStatusBar>
#include <QApplication>
#include <QTableWidgetItem>
#include <QPushButton>
#include <QTimer>
#include <QAbstractItemView>
#include <QScrollBar>
#include <QLineEdit>
#include <QMessageBox>
#include <QTextStream>
#include <QColor>
#include <QRegularExpression>
#include <QStackedWidget>
#include <QBrush>
#include <QGroupBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QDir>
#include <QProgressDialog>
#include <QColorDialog>
#include <QMenu>
#include <QAction>
#include <QInputDialog>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMovie>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>
#include <QRadioButton>
#include <QSet>
#include <QGraphicsDropShadowEffect>
#include <QPainter>
#include <QPainterPath>
#include <QMouseEvent>
#include <QEasingCurve>
#include <QStyle>
#include <QScreen>
#include <QDebug>

// ==================== LOGIN WINDOW IMPLEMENTATION ====================

void LoginWindow::loadSavedCredentials()
{
    QString settingsPath = QApplication::applicationDirPath() + "/credentials.ini";

    if (QFile::exists(settingsPath)) {
        QFile file(settingsPath);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while (!in.atEnd()) {
                QString line = in.readLine();
                if (line.startsWith("AdminUsername="))
                    m_adminUsername = line.mid(14).trimmed();
                else if (line.startsWith("AdminPassword="))
                    m_adminPassword = line.mid(14).trimmed();
                else if (line.startsWith("UserUsername="))
                    m_userUsername = line.mid(13).trimmed();
                else if (line.startsWith("UserPassword="))
                    m_userPassword = line.mid(13).trimmed();
            }
            file.close();
        }
    }

    // Update info label
    if (infoLabel) {
        infoLabel->setText(QString("Default Admin: %1\nDefault User: %2")
                               .arg(m_adminUsername).arg(m_userUsername));
    }
}

void LoginWindow::saveCredentialsToFile()
{
    QString settingsPath = QApplication::applicationDirPath() + "/credentials.ini";
    QFile file(settingsPath);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "AdminUsername=" << m_adminUsername << "\n";
        out << "AdminPassword=" << m_adminPassword << "\n";
        out << "UserUsername=" << m_userUsername << "\n";
        out << "UserPassword=" << m_userPassword << "\n";
        file.close();
    }
}

LoginWindow::LoginWindow(QWidget *parent)
    : QDialog(parent)
    , m_dragging(false)
    , m_loggedInRole("")
    , m_adminUsername("admin")
    , m_adminPassword("1234")
    , m_userUsername("user")
    , m_userPassword("user123")
    , tempAdminUserEdit(nullptr)
    , tempAdminPassEdit(nullptr)
    , tempUserUserEdit(nullptr)
    , tempUserPassEdit(nullptr)
{
    setWindowFlags(Qt::Dialog);
    setModal(true);
    setFixedSize(450, 650);  // Increased height to accommodate edit button
    setWindowTitle("Login - TrainTech Electronics");
    m_backgroundColor = QColor(255, 255, 255);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(30, 30, 30, 30);
    mainLayout->setSpacing(15);

    QPushButton *closeButton = new QPushButton("×");
    closeButton->setFixedSize(32, 32);
    closeButton->setCursor(Qt::PointingHandCursor);
    closeButton->setStyleSheet(
        "QPushButton { background-color: transparent; color: #666; font-size: 20px; font-weight: bold; border: none; border-radius: 16px; }"
        "QPushButton:hover { background-color: #ff4444; color: white; }");
    connect(closeButton, &QPushButton::clicked, this, &QDialog::reject);

    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->addStretch();
    topLayout->addWidget(closeButton);
    mainLayout->addLayout(topLayout);

    logoLabel = new QLabel();
    logoLabel->setFixedSize(80, 80);
    logoLabel->setAlignment(Qt::AlignCenter);
    logoLabel->setStyleSheet("QLabel { background-color: qlineargradient(x1:0, y1:0, x2:1, y2:1, stop:0 #0078d7, stop:1 #00a2ff); border-radius: 40px; }");

    QLabel *iconText = new QLabel("TEL");
    iconText->setStyleSheet("color: white; font-size: 40px; font-weight: bold; background: transparent;");
    iconText->setAlignment(Qt::AlignCenter);
    QVBoxLayout *logoLayout = new QVBoxLayout(logoLabel);
    logoLayout->addWidget(iconText);
    mainLayout->addWidget(logoLabel, 0, Qt::AlignCenter);

    titleLabel = new QLabel("TRAINTECH ELECTRONICS");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 22px; font-weight: bold; color: #1a1a1a; letter-spacing: 1px; margin-top: 10px; }");
    mainLayout->addWidget(titleLabel);

    subtitleLabel = new QLabel("Data Recording System");
    subtitleLabel->setAlignment(Qt::AlignCenter);
    subtitleLabel->setStyleSheet("QLabel { font-size: 13px; color: #666; margin-bottom: 20px; }");
    mainLayout->addWidget(subtitleLabel);

    QLabel *userIconLabel = new QLabel("👤");
    userIconLabel->setStyleSheet("font-size: 16px; background: transparent;");

    usernameEdit = new QLineEdit();
    usernameEdit->setPlaceholderText("Username");
    usernameEdit->setMinimumHeight(45);
    usernameEdit->setStyleSheet("QLineEdit { padding: 8px 12px; padding-left: 40px; font-size: 14px; border: 2px solid #e0e0e0; border-radius: 8px; background-color: #f8f9fa; color: #1a1a1a; } QLineEdit:focus { border-color: #0078d7; background-color: white; }");

    QWidget *usernameContainer = new QWidget();
    QVBoxLayout *usernameContainerLayout = new QVBoxLayout(usernameContainer);
    usernameContainerLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *usernameLayout = new QHBoxLayout();
    usernameLayout->setContentsMargins(10, 0, 10, 0);
    usernameLayout->addWidget(userIconLabel);
    usernameLayout->addWidget(usernameEdit);
    usernameContainerLayout->addLayout(usernameLayout);
    mainLayout->addWidget(usernameContainer);

    QLabel *passIconLabel = new QLabel("🔒");
    passIconLabel->setStyleSheet("font-size: 16px; background: transparent;");

    passwordEdit = new QLineEdit();
    passwordEdit->setPlaceholderText("Password");
    passwordEdit->setEchoMode(QLineEdit::Password);
    passwordEdit->setMinimumHeight(45);
    passwordEdit->setStyleSheet("QLineEdit { padding: 8px 12px; padding-left: 40px; font-size: 14px; border: 2px solid #e0e0e0; border-radius: 8px; background-color: #f8f9fa; color: #1a1a1a; } QLineEdit:focus { border-color: #0078d7; background-color: white; }");

    togglePasswordBtn = new QPushButton("👁");
    togglePasswordBtn->setFixedSize(30, 30);
    togglePasswordBtn->setCursor(Qt::PointingHandCursor);
    togglePasswordBtn->setStyleSheet("QPushButton { background-color: transparent; border: none; font-size: 16px; }");
    connect(togglePasswordBtn, &QPushButton::clicked, this, &LoginWindow::togglePasswordVisibility);

    QWidget *passwordContainer = new QWidget();
    QVBoxLayout *passwordContainerLayout = new QVBoxLayout(passwordContainer);
    passwordContainerLayout->setContentsMargins(0, 0, 0, 0);
    QHBoxLayout *passwordLayout = new QHBoxLayout();
    passwordLayout->setContentsMargins(10, 0, 10, 0);
    passwordLayout->addWidget(passIconLabel);
    passwordLayout->addWidget(passwordEdit);
    passwordLayout->addWidget(togglePasswordBtn);
    passwordContainerLayout->addLayout(passwordLayout);
    mainLayout->addWidget(passwordContainer);

    errorLabel = new QLabel();
    errorLabel->setAlignment(Qt::AlignCenter);
    errorLabel->setVisible(false);
    errorLabel->setStyleSheet("QLabel { color: #dc3545; font-size: 12px; padding: 8px; background-color: #ffe5e5; border-radius: 6px; }");
    mainLayout->addWidget(errorLabel);

    loginButton = new QPushButton("Sign In");
    loginButton->setMinimumHeight(48);
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setStyleSheet("QPushButton { background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0078d7, stop:1 #00a2ff); color: white; font-size: 15px; font-weight: bold; border: none; border-radius: 8px; padding: 10px; } QPushButton:hover { background-color: qlineargradient(x1:0, y1:0, x2:1, y2:0, stop:0 #0086e8, stop:1 #14aeff); }");
    mainLayout->addWidget(loginButton);

    // Add Edit Credentials Button
    QPushButton *editCredentialsBtn = new QPushButton("✏️ Edit Default Credentials");
    editCredentialsBtn->setMinimumHeight(35);
    editCredentialsBtn->setCursor(Qt::PointingHandCursor);
    editCredentialsBtn->setStyleSheet(
        "QPushButton { background-color: #ffc107; color: black; font-size: 13px; font-weight: bold; "
        "border: none; border-radius: 6px; padding: 8px; margin-top: 5px; }"
        "QPushButton:hover { background-color: #e0a800; }");
    connect(editCredentialsBtn, &QPushButton::clicked, this, &LoginWindow::onEditCredentials);
    mainLayout->addWidget(editCredentialsBtn);

    infoLabel = new QLabel();
    infoLabel->setAlignment(Qt::AlignCenter);
    infoLabel->setStyleSheet("QLabel { color: #999; font-size: 11px; margin-top: 10px; }");
    mainLayout->addWidget(infoLabel);

    // Load saved credentials after UI setup
    loadSavedCredentials();

    connect(loginButton, &QPushButton::clicked, this, &LoginWindow::checkLogin);
    errorAnimation = new QPropertyAnimation(this, "backgroundColor");
    errorAnimation->setDuration(300);
    errorAnimation->setEasingCurve(QEasingCurve::InOutQuad);

    QGraphicsDropShadowEffect *windowShadow = new QGraphicsDropShadowEffect();
    windowShadow->setBlurRadius(30);
    windowShadow->setColor(QColor(0, 0, 0, 80));
    windowShadow->setOffset(0, 5);
    setGraphicsEffect(windowShadow);

    usernameEdit->setFocus();
    connect(usernameEdit, &QLineEdit::returnPressed, this, &LoginWindow::checkLogin);
    connect(passwordEdit, &QLineEdit::returnPressed, this, &LoginWindow::checkLogin);
}

void LoginWindow::onEditCredentials()
{
    bool ok;

    QString password = QInputDialog::getText(
        this,
        "Admin Authentication",
        "Enter Admin Password:",
        QLineEdit::Password,
        "",
        &ok
        );

    if (!ok)
        return;

    if (password != m_adminPassword)
    {
        QMessageBox::warning(
            this,
            "Access Denied",
            "Incorrect Admin Password!"
            );
        return;
    }

    QDialog editDialog(this);
    editDialog.setWindowTitle("Edit Default Credentials");
    editDialog.setModal(true);
    editDialog.setMinimumSize(400, 400);
    editDialog.setStyleSheet("QDialog { background-color: #f8f9fa; }");

    QVBoxLayout *layout = new QVBoxLayout(&editDialog);

    QLabel *titleLabel = new QLabel("Change Default Login Credentials");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #0078d7; margin: 10px; }");
    layout->addWidget(titleLabel);

    QLabel *warningLabel = new QLabel("⚠️ These credentials will be used for future logins");
    warningLabel->setAlignment(Qt::AlignCenter);
    warningLabel->setStyleSheet("QLabel { color: #ff9800; font-size: 12px; padding: 5px; background-color: #fff3e0; border-radius: 4px; }");
    layout->addWidget(warningLabel);

    // Admin Section
    QGroupBox *adminGroup = new QGroupBox("Administrator Account");
    adminGroup->setStyleSheet("QGroupBox { font-weight: bold; border: 2px solid #28a745; border-radius: 8px; margin-top: 10px; padding-top: 10px; }");
    QFormLayout *adminLayout = new QFormLayout(adminGroup);

    tempAdminUserEdit = new QLineEdit(m_adminUsername);
    tempAdminUserEdit->setPlaceholderText("Admin Username");
    tempAdminUserEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; }");

    tempAdminPassEdit = new QLineEdit(m_adminPassword);
    tempAdminPassEdit->setEchoMode(QLineEdit::Password);
    tempAdminPassEdit->setPlaceholderText("Admin Password");
    tempAdminPassEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; }");

    QPushButton *toggleAdminPass = new QPushButton("👁 Show/Hide");
    toggleAdminPass->setFixedSize(80, 30);
    connect(toggleAdminPass, &QPushButton::clicked, [this]() {
        if (tempAdminPassEdit->echoMode() == QLineEdit::Password)
            tempAdminPassEdit->setEchoMode(QLineEdit::Normal);
        else
            tempAdminPassEdit->setEchoMode(QLineEdit::Password);
    });

    adminLayout->addRow("Username:", tempAdminUserEdit);
    adminLayout->addRow("Password:", tempAdminPassEdit);
    adminLayout->addRow("", toggleAdminPass);
    layout->addWidget(adminGroup);

    // User Section
    QGroupBox *userGroup = new QGroupBox("User Account");
    userGroup->setStyleSheet("QGroupBox { font-weight: bold; border: 2px solid #17a2b8; border-radius: 8px; margin-top: 10px; padding-top: 10px; }");
    QFormLayout *userLayout = new QFormLayout(userGroup);

    tempUserUserEdit = new QLineEdit(m_userUsername);
    tempUserUserEdit->setPlaceholderText("User Username");
    tempUserUserEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; }");

    tempUserPassEdit = new QLineEdit(m_userPassword);
    tempUserPassEdit->setEchoMode(QLineEdit::Password);
    tempUserPassEdit->setPlaceholderText("User Password");
    tempUserPassEdit->setStyleSheet("QLineEdit { padding: 8px; border: 1px solid #ccc; border-radius: 4px; }");

    QPushButton *toggleUserPass = new QPushButton("👁 Show/Hide");
    toggleUserPass->setFixedSize(80, 30);
    connect(toggleUserPass, &QPushButton::clicked, [this]() {
        if (tempUserPassEdit->echoMode() == QLineEdit::Password)
            tempUserPassEdit->setEchoMode(QLineEdit::Normal);
        else
            tempUserPassEdit->setEchoMode(QLineEdit::Password);
    });

    userLayout->addRow("Username:", tempUserUserEdit);
    userLayout->addRow("Password:", tempUserPassEdit);
    userLayout->addRow("", toggleUserPass);
    layout->addWidget(userGroup);

    // Password Requirements
    QLabel *reqLabel = new QLabel("Password Requirements:\n• Minimum 4 characters\n• Cannot be empty");
    reqLabel->setStyleSheet("QLabel { color: #666; font-size: 11px; padding: 5px; background-color: #e9ecef; border-radius: 4px; }");
    layout->addWidget(reqLabel);

    // Buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    QPushButton *saveButton = new QPushButton("💾 Save Changes");
    saveButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 10px; border-radius: 5px; font-weight: bold; }");

    QPushButton *cancelButton = new QPushButton("❌ Cancel");
    cancelButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 10px; border-radius: 5px; font-weight: bold; }");

    QPushButton *resetButton = new QPushButton("🔄 Reset to Default");
    resetButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; padding: 10px; border-radius: 5px; font-weight: bold; }");

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(resetButton);
    layout->addLayout(buttonLayout);

    // Connect buttons
    connect(saveButton, &QPushButton::clicked, [&]() {
        QString newAdminUser = tempAdminUserEdit->text().trimmed();
        QString newAdminPass = tempAdminPassEdit->text().trimmed();
        QString newUserUser = tempUserUserEdit->text().trimmed();
        QString newUserPass = tempUserPassEdit->text().trimmed();

        if (validateCredentials(newAdminUser, newAdminPass, newUserUser, newUserPass)) {
            // Save credentials locally in LoginWindow
            m_adminUsername = newAdminUser;
            m_adminPassword = newAdminPass;
            m_userUsername = newUserUser;
            m_userPassword = newUserPass;

            // Save to persistent file
            saveCredentialsToFile();

            // Update info label
            infoLabel->setText(QString("Default Admin: %1\nDefault User: %2")
                                   .arg(m_adminUsername).arg(m_userUsername));

            // Also update MainWindow's credentials
            MainWindow *mainWin = qobject_cast<MainWindow*>(parent());
            if (mainWin) {
                mainWin->changeDefaultCredentials(newAdminUser, newAdminPass, newUserUser, newUserPass);
            }

            QMessageBox::information(&editDialog, "Success",
                                     "Default credentials have been updated!\n\n"
                                     "New credentials will be used for future logins.\n\n"
                                     "Admin: " + m_adminUsername + "\n"
                                                             "User: " + m_userUsername);

            editDialog.accept();
        }
    });

    connect(cancelButton, &QPushButton::clicked, &editDialog, &QDialog::reject);

    connect(resetButton, &QPushButton::clicked, [&]() {
        QMessageBox::StandardButton reply = QMessageBox::question(&editDialog, "Reset to Default",
                                                                  "Are you sure you want to reset to factory default credentials?\n\n"
                                                                  "Admin: admin / 1234\nUser: user / user123",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            tempAdminUserEdit->setText("admin");
            tempAdminPassEdit->setText("1234");
            tempUserUserEdit->setText("user");
            tempUserPassEdit->setText("user123");
        }
    });

    editDialog.exec();
}

bool LoginWindow::validateCredentials(const QString& adminUser, const QString& adminPass,
                                      const QString& userUser, const QString& userPass)
{
    if (adminUser.isEmpty() || adminPass.isEmpty() || userUser.isEmpty() || userPass.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "All fields must be filled!");
        return false;
    }

    if (adminPass.length() < 4) {
        QMessageBox::warning(this, "Weak Password", "Admin password must be at least 4 characters long!");
        return false;
    }

    if (userPass.length() < 4) {
        QMessageBox::warning(this, "Weak Password", "User password must be at least 4 characters long!");
        return false;
    }

    if (adminUser == userUser) {
        QMessageBox::warning(this, "Invalid Input", "Admin and User usernames cannot be the same!");
        return false;
    }

    return true;
}

void LoginWindow::updateCredentials(const QString& adminUser, const QString& adminPass,
                                    const QString& regUser, const QString& regPass)
{
    m_adminUsername = adminUser;
    m_adminPassword = adminPass;
    m_userUsername = regUser;
    m_userPassword = regPass;

    infoLabel->setText(QString("Default Admin: %1\nDefault User: %2")
                           .arg(m_adminUsername).arg(m_userUsername));

    // Save to persistent file
    saveCredentialsToFile();
}

QString LoginWindow::loggedInRole() const
{
    return m_loggedInRole;
}

void LoginWindow::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    QPainterPath path;
    path.addRoundedRect(rect(), 15, 15);
    painter.fillPath(path, m_backgroundColor);
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(0, 120, 215, 50));
    gradient.setColorAt(1, QColor(0, 162, 255, 50));
    painter.setPen(QPen(gradient, 2));
    painter.drawPath(path);
}

void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - frameGeometry().topLeft();
        event->accept();
    }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_dragging = false;
}

void LoginWindow::checkLogin()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        errorLabel->setText("❌ Please enter both username and password");
        errorLabel->setVisible(true);
        animateError();
        return;
    }

    if (username == m_adminUsername && password == m_adminPassword) {
        m_loggedInRole = "Admin";
        accept();
    } else if (username == m_userUsername && password == m_userPassword) {
        m_loggedInRole = "User";
        accept();
    } else {
        errorLabel->setText("❌ Invalid username or password");
        errorLabel->setVisible(true);
        animateError();
        passwordEdit->clear();
        passwordEdit->setFocus();

        QPropertyAnimation *shake = new QPropertyAnimation(this, "pos");
        shake->setDuration(100);
        shake->setLoopCount(3);
        QPoint currentPos = pos();
        shake->setKeyValueAt(0, currentPos);
        shake->setKeyValueAt(0.1, currentPos + QPoint(10, 0));
        shake->setKeyValueAt(0.2, currentPos - QPoint(10, 0));
        shake->setKeyValueAt(1, currentPos);
        shake->start(QAbstractAnimation::DeleteWhenStopped);
    }
}

void LoginWindow::togglePasswordVisibility()
{
    if (passwordEdit->echoMode() == QLineEdit::Password) {
        passwordEdit->setEchoMode(QLineEdit::Normal);
        togglePasswordBtn->setText("🙈");
    } else {
        passwordEdit->setEchoMode(QLineEdit::Password);
        togglePasswordBtn->setText("👁");
    }
}

void LoginWindow::animateError()
{
    errorAnimation->setStartValue(QColor(255, 255, 255));
    errorAnimation->setEndValue(QColor(255, 229, 229));
    errorAnimation->start();
    QTimer::singleShot(3000, this, [this]() {
        errorLabel->setVisible(false);
        setBackgroundColor(QColor(255, 255, 255));
    });
}

// ==================== MAIN WINDOW IMPLEMENTATION ====================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , frozenTableWidget(nullptr)
    , dataTableWidget(nullptr)
    , pushButtonBrowse(nullptr)
    , exportButton(nullptr)
    , loginButton(nullptr)
    , findButton(nullptr)
    , clearSearchButton(nullptr)
    , m_clearHighlightsButton(nullptr)
    , m_clearFiltersButton(nullptr)
    , m_resetSettingsButton(nullptr)
    , btnDataView(nullptr)
    , btnSettings(nullptr)
    , saveSettingsButton(nullptr)
    , rowHighlightColorButton(nullptr)
    , columnHighlightColorButton(nullptr)
    , searchEdit(nullptr)
    , adminUsernameEdit(nullptr)
    , adminPasswordEdit(nullptr)
    , userUsernameEdit(nullptr)
    , userPasswordEdit(nullptr)
    , enableHighlightingCheckbox(nullptr)
    , lightThemeRadio(nullptr)
    , darkThemeRadio(nullptr)
    , stackedWidget(nullptr)
    , m_loadingOverlay(nullptr)
    , m_loadingLabel(nullptr)
    , m_loadingProgressLabel(nullptr)
    , m_loadingProgressBar(nullptr)
    , m_loadingMovie(nullptr)
    , m_loadingActive(false)
    , m_maxRowsLoaded(0)
    , m_currentHighlightedRow(-1)
    , m_currentHighlightedColumn(-1)
    , m_rowHighlightColor(173, 216, 230)
    , m_columnHighlightColor(100, 149, 237)
    , m_highlightingEnabled(true)
    , m_currentRole("Guest")
    , m_currentTheme("Light")
    , m_adminUsername("admin")
    , m_adminPassword("1234")
    , m_userUsername("user")
    , m_userPassword("user123")
    , m_currentCacheStartRow(0)
    , m_currentCacheEndRow(0)
{
    setupUI();
    loadSettings();
    QTimer::singleShot(100, this, &MainWindow::loadDefaultFile);
    applyInitialTheme();
    statusBar()->showMessage("Welcome to TrainTech Electronics Data Viewer", 5000);

    // Ensure highlighting is enabled
    m_highlightingEnabled = true;
    if (enableHighlightingCheckbox) {
        enableHighlightingCheckbox->setChecked(true);
    }
}

MainWindow::~MainWindow()
{
    if (m_loadingMovie) delete m_loadingMovie;
    clearTables();
    m_columnFilters.clear();
    m_columnUniqueValues.clear();
    m_cachedData.clear();
    if (m_currentRole == "Admin") saveSettingsInternal();
}

void MainWindow::changeDefaultCredentials(const QString& newAdminUser, const QString& newAdminPass,
                                          const QString& newUserUser, const QString& newUserPass)
{
    m_adminUsername = newAdminUser;
    m_adminPassword = newAdminPass;
    m_userUsername = newUserUser;
    m_userPassword = newUserPass;

    // Update the UI text fields if they exist
    if (adminUsernameEdit) adminUsernameEdit->setText(m_adminUsername);
    if (adminPasswordEdit) adminPasswordEdit->setText(m_adminPassword);
    if (userUsernameEdit) userUsernameEdit->setText(m_userUsername);
    if (userPasswordEdit) userPasswordEdit->setText(m_userPassword);

    // Save to settings file
    saveSettingsInternal();

    statusBar()->showMessage("Default credentials updated successfully!", 3000);
}

void MainWindow::onChangeDefaultCredentials()
{
    if (m_currentRole != "Admin") {
        QMessageBox::warning(this, "Access Denied",
                             "Only administrators can change default credentials.\n\n"
                             "Please login as Admin first.");
        return;
    }

    QDialog credDialog(this);
    credDialog.setWindowTitle("Change Default Login Credentials");
    credDialog.setModal(true);
    credDialog.setMinimumSize(400, 400);

    QVBoxLayout *layout = new QVBoxLayout(&credDialog);

    QLabel *titleLabel = new QLabel("Change Default Login Credentials");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("QLabel { font-size: 16px; font-weight: bold; color: #0078d7; margin-bottom: 20px; }");
    layout->addWidget(titleLabel);

    QGroupBox *adminGroup = new QGroupBox("Administrator Credentials");
    QFormLayout *adminLayout = new QFormLayout(adminGroup);

    QLineEdit *adminUserEdit = new QLineEdit(m_adminUsername);
    adminUserEdit->setPlaceholderText("Admin Username");

    QLineEdit *adminPassEdit = new QLineEdit(m_adminPassword);
    adminPassEdit->setEchoMode(QLineEdit::Password);
    adminPassEdit->setPlaceholderText("Admin Password");

    QPushButton *toggleAdminPass = new QPushButton("👁 Show/Hide");
    toggleAdminPass->setFixedSize(80, 25);
    connect(toggleAdminPass, &QPushButton::clicked, [adminPassEdit]() {
        adminPassEdit->setEchoMode(adminPassEdit->echoMode() == QLineEdit::Password
                                       ? QLineEdit::Normal : QLineEdit::Password);
    });

    adminLayout->addRow("Username:", adminUserEdit);
    adminLayout->addRow("Password:", adminPassEdit);
    adminLayout->addRow("", toggleAdminPass);
    layout->addWidget(adminGroup);

    QGroupBox *userGroup = new QGroupBox("User Credentials");
    QFormLayout *userLayout = new QFormLayout(userGroup);

    QLineEdit *userUserEdit = new QLineEdit(m_userUsername);
    userUserEdit->setPlaceholderText("User Username");

    QLineEdit *userPassEdit = new QLineEdit(m_userPassword);
    userPassEdit->setEchoMode(QLineEdit::Password);
    userPassEdit->setPlaceholderText("User Password");

    QPushButton *toggleUserPass = new QPushButton("👁 Show/Hide");
    toggleUserPass->setFixedSize(80, 25);
    connect(toggleUserPass, &QPushButton::clicked, [userPassEdit]() {
        userPassEdit->setEchoMode(userPassEdit->echoMode() == QLineEdit::Password
                                      ? QLineEdit::Normal : QLineEdit::Password);
    });

    userLayout->addRow("Username:", userUserEdit);
    userLayout->addRow("Password:", userPassEdit);
    userLayout->addRow("", toggleUserPass);
    layout->addWidget(userGroup);

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *saveButton = new QPushButton("Save Changes");
    saveButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 8px; border-radius: 5px; }");

    QPushButton *cancelButton = new QPushButton("Cancel");
    cancelButton->setStyleSheet("QPushButton { background-color: #6c757d; color: white; padding: 8px; border-radius: 5px; }");

    QPushButton *resetDefaultButton = new QPushButton("Reset to Factory Default");
    resetDefaultButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; padding: 8px; border-radius: 5px; }");

    buttonLayout->addWidget(saveButton);
    buttonLayout->addWidget(cancelButton);
    buttonLayout->addWidget(resetDefaultButton);
    layout->addLayout(buttonLayout);

    connect(saveButton, &QPushButton::clicked, [&]() {
        QString newAdminUser = adminUserEdit->text().trimmed();
        QString newAdminPass = adminPassEdit->text().trimmed();
        QString newUserUser = userUserEdit->text().trimmed();
        QString newUserPass = userPassEdit->text().trimmed();

        if (newAdminUser.isEmpty() || newAdminPass.isEmpty() ||
            newUserUser.isEmpty() || newUserPass.isEmpty()) {
            QMessageBox::warning(&credDialog, "Invalid Input", "All fields must be filled!");
            return;
        }

        if (newAdminPass.length() < 4) {
            QMessageBox::warning(&credDialog, "Weak Password", "Admin password must be at least 4 characters!");
            return;
        }

        if (newUserPass.length() < 4) {
            QMessageBox::warning(&credDialog, "Weak Password", "User password must be at least 4 characters!");
            return;
        }

        changeDefaultCredentials(newAdminUser, newAdminPass, newUserUser, newUserPass);

        QMessageBox::information(&credDialog, "Success",
                                 "Default credentials have been updated!\n\n"
                                 "New credentials will be used for future logins.");
        credDialog.accept();
    });

    connect(cancelButton, &QPushButton::clicked, &credDialog, &QDialog::reject);

    connect(resetDefaultButton, &QPushButton::clicked, [&]() {
        QMessageBox::StandardButton reply = QMessageBox::question(&credDialog, "Reset to Default",
                                                                  "Are you sure you want to reset to factory default credentials?\n\n"
                                                                  "Admin: admin / 1234\nUser: user / user123",
                                                                  QMessageBox::Yes | QMessageBox::No);

        if (reply == QMessageBox::Yes) {
            adminUserEdit->setText("admin");
            adminPassEdit->setText("1234");
            userUserEdit->setText("user");
            userPassEdit->setText("user123");
        }
    });

    credDialog.exec();
}

void MainWindow::setupUI()
{
    resize(1600, 750);
    QWidget *central = new QWidget(this);
    setCentralWidget(central);
    QVBoxLayout *mainLayout = new QVBoxLayout(central);
    mainLayout->setSpacing(2);
    mainLayout->setContentsMargins(4, 4, 4, 4);
    setupTopToolbar(mainLayout);
    setupMenuButtons(mainLayout);
    setupMainContent(mainLayout);
    setupConnections();
    setupLoadingOverlay();
}
void MainWindow::setupTopToolbar(QVBoxLayout *mainLayout)
{
    QHBoxLayout *topLayout = new QHBoxLayout();
    topLayout->setContentsMargins(0, 0, 0, 0);
    topLayout->setSpacing(6);
    QLabel *companyLabel = new QLabel("TRAINTECH ELECTRONICS PVT.LTD");
    companyLabel->setAlignment(Qt::AlignCenter);
    companyLabel->setStyleSheet("QLabel { color: #00A2FF; font-size: 18px; font-weight: bold; padding: 4px 10px; }");

    pushButtonBrowse = createButton("Browse File", 30);
    exportButton = createButton("Export CSV", 30);

    QPushButton *clearFiltersButton = createButton("Clear All Filters", 30);
    clearFiltersButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; }");

    loginButton = createButton("🔐 Login", 30);
    loginButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 4px; border-radius: 4px; font-weight: bold; }");
    loginButton->setCursor(Qt::PointingHandCursor);
    loginButton->setToolTip("Click to login as Admin or User");

    searchEdit = new QLineEdit();
    searchEdit->setPlaceholderText("Search...");
    searchEdit->setFixedHeight(30);
    searchEdit->setMinimumWidth(200);
    searchEdit->setStyleSheet("QLineEdit { color: #000000; background-color: white; padding: 4px 8px; border: 1px solid #cfcfcf; border-radius: 4px; }");

    findButton = createButton("Find", 30, QSize(60, 30));
    clearSearchButton = createButton("Clear", 30, QSize(60, 30));
    m_clearHighlightsButton = createButton("Clear Highlights", 30, QSize(120, 30));

    topLayout->addWidget(companyLabel);
    topLayout->addStretch();
    topLayout->addWidget(searchEdit);
    topLayout->addWidget(findButton);
    topLayout->addWidget(clearSearchButton);
    topLayout->addWidget(m_clearHighlightsButton);
    topLayout->addWidget(clearFiltersButton);
    topLayout->addWidget(pushButtonBrowse);
    topLayout->addWidget(exportButton);
    topLayout->addWidget(loginButton);

    mainLayout->addLayout(topLayout);
    m_clearFiltersButton = clearFiltersButton;
}
void MainWindow::setupMenuButtons(QVBoxLayout *mainLayout)
{
    QHBoxLayout *menuButtonsLayout = new QHBoxLayout();
    menuButtonsLayout->setSpacing(8);

    btnDataView = new QPushButton("📊 Data View");
    btnSettings = new QPushButton("⚙️ Settings");

    QString menuButtonStyle = "QPushButton { padding: 6px 16px; border-radius: 5px; background-color: #e0e0e0; } QPushButton:checked { background-color: #0078d7; color: white; }";
    btnDataView->setStyleSheet(menuButtonStyle);
    btnSettings->setStyleSheet(menuButtonStyle);
    btnDataView->setCheckable(true);
    btnSettings->setCheckable(true);
    btnDataView->setChecked(true);

    menuButtonsLayout->addStretch();
    menuButtonsLayout->addWidget(btnDataView);
    menuButtonsLayout->addWidget(btnSettings);
    menuButtonsLayout->addStretch();

    mainLayout->addLayout(menuButtonsLayout);
}

void MainWindow::setupMainContent(QVBoxLayout *mainLayout)
{
    QHBoxLayout *mainContentLayout = new QHBoxLayout();
    mainContentLayout->setContentsMargins(0, 0, 0, 0);
    stackedWidget = new QStackedWidget();
    mainContentLayout->addWidget(stackedWidget, 1);
    setupDataPage();
    setupSettingsPage();
    mainLayout->addLayout(mainContentLayout);
}

void MainWindow::setupDataPage()
{
    QWidget *dataPage = new QWidget;
    QVBoxLayout *dataLayout = new QVBoxLayout(dataPage);
    dataLayout->setContentsMargins(0, 0, 0, 0);

    QHBoxLayout *tableLayout = new QHBoxLayout();
    tableLayout->setSpacing(0);
    tableLayout->setContentsMargins(0, 0, 0, 0);

    setupFrozenTable();
    setupDataTable();

    connect(frozenTableWidget->verticalScrollBar(), &QScrollBar::valueChanged,
            dataTableWidget->verticalScrollBar(), &QScrollBar::setValue);
    connect(dataTableWidget->verticalScrollBar(), &QScrollBar::valueChanged,
            frozenTableWidget->verticalScrollBar(), &QScrollBar::setValue);

    tableLayout->addWidget(frozenTableWidget);
    tableLayout->addWidget(dataTableWidget);
    tableLayout->setStretchFactor(frozenTableWidget, 0);
    tableLayout->setStretchFactor(dataTableWidget, 1);

    dataLayout->addLayout(tableLayout);
    frozenTableWidget->setFixedWidth(200);
    stackedWidget->addWidget(dataPage);
}

void MainWindow::setupFrozenTable()
{
    frozenTableWidget = new QTableWidget();
    frozenTableWidget->setSortingEnabled(true);
    frozenTableWidget->setSelectionBehavior(QAbstractItemView::SelectItems);
    frozenTableWidget->setAlternatingRowColors(true);
    frozenTableWidget->verticalHeader()->setVisible(false);
    frozenTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    frozenTableWidget->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    frozenTableWidget->setSelectionMode(QAbstractItemView::NoSelection);  // Disable default selection

    frozenTableWidget->setStyleSheet(
        "QTableWidget { background:white; color:black; gridline-color:#d0d0d0; }"
        "QTableWidget::item:selected { background-color: transparent; }"
        "QTableWidget::item:focus { outline: none; }");

    frozenTableWidget->setColumnCount(2);
    QStringList frozenHeaders = {"DATE", "TIME"};
    frozenTableWidget->setHorizontalHeaderLabels(frozenHeaders);
    frozenTableWidget->setColumnWidth(0, FROZEN_COLUMN_WIDTH_DATE);
    frozenTableWidget->setColumnWidth(1, FROZEN_COLUMN_WIDTH_TIME);
}

void MainWindow::setupDataTable()
{
    dataTableWidget = new QTableWidget();
    headers.clear();   // Add this line

    headers << "DATE" << "TIME" << "SPEED" << "OHE_VOLT" << "OHE_CURR" << "ENER_CONS"
            << "BAT_VOLT" << "BP_PRES" << "TE_BE_DEM" << "TE_BE_BG1" << "TE_BE_BG2"
            << "CAB1_FWSAND" << "CAB2_FWSAND" << "CAB1_KEY_D" << "CAB2_KEY_D"
            << "COMPRESSOR" << "CAB1_DIRFOR" << "CAB1_DIRREV" << "CAB1_MAXTE" << "CAB2_MAXTE"
            << "CAB1_EMGSTOP" << "CAB2_EMGSTOP" << "CAB1_FTSWLOCBRK" << "CAB2_FTSWLOCBRK"
            << "CAB2_DIRFOR" << "CAB2_DIRREV" << "PANTO1_PRSW" << "PANTO2_PRSW"
            << "VCB_STS" << "MOTORING" << "REGEN_BRK" << "EMRG_BRK" << "ISOL_EMG_EXH"
            << "START_RUN_INTLK" << "TRACT_INTLK" << "NODE_NUM" << "VIG_WARN"
            << "VIG_EMG_BRK" << "AFLOW_PRSW" << "BC1_PRSW" << "BC2_PRSW"
            << "MR_LOW_PRSW" << "BRK_ELE_HLTH" << "LOCO_NUM" << "LSP_STS";

    dataTableWidget->setColumnCount(43);
    QStringList dataHeaders = headers.mid(2);
    dataTableWidget->setHorizontalHeaderLabels(dataHeaders);

    for(int i = 0; i < 43; i++) {
        dataTableWidget->setColumnWidth(i, DEFAULT_COLUMN_WIDTH);
    }
}
void MainWindow::setupSettingsPage()
{
    QWidget *settingsPage = new QWidget;
    QVBoxLayout *settingsLayout = new QVBoxLayout(settingsPage);
    settingsLayout->setSpacing(15);
    settingsLayout->setContentsMargins(30, 30, 30, 30);

    QLabel *settingsTitle = new QLabel("⚙️ Settings");
    settingsTitle->setAlignment(Qt::AlignCenter);
    settingsTitle->setStyleSheet("QLabel { font-size: 20px; font-weight: bold; color: #0078d7; }");
    settingsLayout->addWidget(settingsTitle);

    QLabel *editNote = new QLabel("Note: Login as Admin to edit credentials below");
    editNote->setAlignment(Qt::AlignCenter);
    editNote->setStyleSheet("QLabel { color: #ff9800; font-size: 12px; padding: 5px; background-color: #fff3e0; border-radius: 4px; }");
    settingsLayout->addWidget(editNote);

    setupAdminGroup(settingsLayout);
    setupUserGroup(settingsLayout);
    setupThemeGroup(settingsLayout);
    setupHighlightingGroup(settingsLayout);
    setupSettingsButtons(settingsLayout);
    stackedWidget->addWidget(settingsPage);
}

void MainWindow::setupAdminGroup(QVBoxLayout *settingsLayout)
{
    QGroupBox *adminGroup = createGroupBox("Administrator Account");
    QFormLayout *adminLayout = new QFormLayout(adminGroup);
    adminLayout->setSpacing(10);
    adminLayout->setContentsMargins(15, 15, 15, 15);

    adminUsernameEdit = new QLineEdit(m_adminUsername);
    adminUsernameEdit->setMinimumHeight(30);
    adminUsernameEdit->setPlaceholderText("Admin Username");
    adminUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; }");

    adminPasswordEdit = new QLineEdit(m_adminPassword);
    adminPasswordEdit->setEchoMode(QLineEdit::Password);
    adminPasswordEdit->setMinimumHeight(30);
    adminPasswordEdit->setPlaceholderText("Admin Password");
    adminPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; }");

    adminLayout->addRow("Username:", adminUsernameEdit);
    adminLayout->addRow("Password:", adminPasswordEdit);

    QPushButton *toggleAdminPass = new QPushButton("👁 Show/Hide");
    toggleAdminPass->setFixedSize(80, 25);
    toggleAdminPass->setCursor(Qt::PointingHandCursor);
    connect(toggleAdminPass, &QPushButton::clicked, [this]() {
        if (adminPasswordEdit->echoMode() == QLineEdit::Password)
            adminPasswordEdit->setEchoMode(QLineEdit::Normal);
        else
            adminPasswordEdit->setEchoMode(QLineEdit::Password);
    });
    adminLayout->addRow("", toggleAdminPass);

    settingsLayout->addWidget(adminGroup);
}

void MainWindow::setupUserGroup(QVBoxLayout *settingsLayout)
{
    QGroupBox *userGroup = createGroupBox("User Account");
    QFormLayout *userLayout = new QFormLayout(userGroup);
    userLayout->setSpacing(10);
    userLayout->setContentsMargins(15, 15, 15, 15);

    userUsernameEdit = new QLineEdit(m_userUsername);
    userUsernameEdit->setMinimumHeight(30);
    userUsernameEdit->setPlaceholderText("User Username");
    userUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; }");

    userPasswordEdit = new QLineEdit(m_userPassword);
    userPasswordEdit->setEchoMode(QLineEdit::Password);
    userPasswordEdit->setMinimumHeight(30);
    userPasswordEdit->setPlaceholderText("User Password");
    userPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; }");

    userLayout->addRow("Username:", userUsernameEdit);
    userLayout->addRow("Password:", userPasswordEdit);

    QPushButton *toggleUserPass = new QPushButton("👁 Show/Hide");
    toggleUserPass->setFixedSize(80, 25);
    toggleUserPass->setCursor(Qt::PointingHandCursor);
    connect(toggleUserPass, &QPushButton::clicked, [this]() {
        if (userPasswordEdit->echoMode() == QLineEdit::Password)
            userPasswordEdit->setEchoMode(QLineEdit::Normal);
        else
            userPasswordEdit->setEchoMode(QLineEdit::Password);
    });
    userLayout->addRow("", toggleUserPass);

    settingsLayout->addWidget(userGroup);
}

void MainWindow::setupThemeGroup(QVBoxLayout *settingsLayout)
{
    QGroupBox *themeGroup = createGroupBox("Appearance");
    QHBoxLayout *themeLayout = new QHBoxLayout(themeGroup);
    lightThemeRadio = new QRadioButton("Light Theme");
    darkThemeRadio = new QRadioButton("Dark Theme");
    lightThemeRadio->setChecked(true);
    themeLayout->addWidget(lightThemeRadio);
    themeLayout->addWidget(darkThemeRadio);
    themeLayout->addStretch();
    settingsLayout->addWidget(themeGroup);
}

void MainWindow::setupHighlightingGroup(QVBoxLayout *settingsLayout)
{
    QGroupBox *highlightGroup = createGroupBox("Row/Column Highlighting");
    QVBoxLayout *highlightLayout = new QVBoxLayout(highlightGroup);
    enableHighlightingCheckbox = new QCheckBox("Enable Row/Column Highlighting");
    enableHighlightingCheckbox->setChecked(m_highlightingEnabled);
    QHBoxLayout *colorLayout = new QHBoxLayout();
    rowHighlightColorButton = createButton("Row Highlight Color", 30);
    columnHighlightColorButton = createButton("Column Highlight Color", 30);
    colorLayout->addWidget(rowHighlightColorButton);
    colorLayout->addWidget(columnHighlightColorButton);
    colorLayout->addStretch();
    highlightLayout->addWidget(enableHighlightingCheckbox);
    highlightLayout->addLayout(colorLayout);
    settingsLayout->addWidget(highlightGroup);
}
void MainWindow::setupSettingsButtons(QVBoxLayout *settingsLayout)
{
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    saveSettingsButton = new QPushButton("💾 Save Settings");
    saveSettingsButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; font-weight: bold; border-radius: 6px; padding: 8px; }");
    saveSettingsButton->setCursor(Qt::PointingHandCursor);

    m_resetSettingsButton = new QPushButton("🔄 Reset to Default");
    m_resetSettingsButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; font-weight: bold; border-radius: 6px; padding: 8px; }");
    m_resetSettingsButton->setCursor(Qt::PointingHandCursor);

    QPushButton *changeDefaultCredButton = new QPushButton("🔐 Change Default Credentials");
    changeDefaultCredButton->setStyleSheet("QPushButton { background-color: #0078d7; color: white; font-weight: bold; border-radius: 6px; padding: 8px; }");
    changeDefaultCredButton->setCursor(Qt::PointingHandCursor);
    connect(changeDefaultCredButton, &QPushButton::clicked, this, &MainWindow::onChangeDefaultCredentials);

    buttonLayout->addWidget(saveSettingsButton);
    buttonLayout->addWidget(m_resetSettingsButton);
    buttonLayout->addWidget(changeDefaultCredButton);
    buttonLayout->addStretch();
    settingsLayout->addLayout(buttonLayout);
    settingsLayout->addStretch();
}
void MainWindow::setupConnections()
{
    connect(pushButtonBrowse, &QPushButton::clicked, this, &MainWindow::browseFile);
    connect(exportButton, &QPushButton::clicked, this, &MainWindow::exportCSV);
    connect(findButton, &QPushButton::clicked, this, &MainWindow::findText);
    connect(clearSearchButton, &QPushButton::clicked, this, &MainWindow::clearSearch);
    connect(btnDataView, &QPushButton::clicked, [this]() { switchToPage(0); });
    connect(btnSettings, &QPushButton::clicked, [this]() { switchToPage(1); });
    connect(dataTableWidget, &QTableWidget::cellClicked, this, &MainWindow::onTableCellClicked);
    connect(frozenTableWidget, &QTableWidget::cellClicked, this, &MainWindow::onFrozenTableCellClicked);
    connect(m_clearHighlightsButton, &QPushButton::clicked, this, &MainWindow::clearRowColumnHighlights);
    connect(m_clearFiltersButton, &QPushButton::clicked, this, &MainWindow::clearAllFilters);
    connect(saveSettingsButton, &QPushButton::clicked, this, &MainWindow::saveSettings);
    connect(m_resetSettingsButton, &QPushButton::clicked, this, &MainWindow::resetSettings);
    connect(lightThemeRadio, &QRadioButton::toggled, this, &MainWindow::applyTheme);
    connect(enableHighlightingCheckbox, &QCheckBox::toggled, this, &MainWindow::toggleHighlighting);
    connect(rowHighlightColorButton, &QPushButton::clicked, this, &MainWindow::changeRowHighlightColor);
    connect(columnHighlightColorButton, &QPushButton::clicked, this, &MainWindow::changeColumnHighlightColor);
    connect(dataTableWidget->horizontalHeader(), &QHeaderView::sectionClicked, this, &MainWindow::onHeaderClicked);
    connect(loginButton, &QPushButton::clicked, this, &MainWindow::showLoginDialog);
    connect(searchEdit, &QLineEdit::returnPressed, this, &MainWindow::findText);
}

QPushButton* MainWindow::createButton(const QString& text, int height, const QSize& fixedSize)
{
    QPushButton* button = new QPushButton(text);
    button->setFixedHeight(height);
    if (fixedSize.isValid()) button->setFixedSize(fixedSize);
    return button;
}

QGroupBox* MainWindow::createGroupBox(const QString& title)
{
    QGroupBox* groupBox = new QGroupBox(title);
    groupBox->setStyleSheet("QGroupBox { font-weight: bold; border: 2px solid #cccccc; border-radius: 8px; margin-top: 8px; padding-top: 8px; } QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }");
    return groupBox;
}

void MainWindow::setupLoadingOverlay()
{
    m_loadingOverlay = new QWidget(this);
    m_loadingOverlay->setStyleSheet("QWidget { background-color: rgba(0, 0, 0, 0.7); border-radius: 10px; }");
    m_loadingOverlay->hide();

    QVBoxLayout *overlayLayout = new QVBoxLayout(m_loadingOverlay);
    overlayLayout->setAlignment(Qt::AlignCenter);

    QLabel *movieLabel = new QLabel();
    movieLabel->setFixedSize(64, 64);
    movieLabel->setStyleSheet("QLabel { border: 6px solid #f3f3f3; border-top: 6px solid #3498db; border-radius: 32px; }");

    overlayLayout->addWidget(movieLabel, 0, Qt::AlignCenter);

    m_loadingLabel = new QLabel("Loading file...");
    m_loadingLabel->setStyleSheet("QLabel { color: white; font-size: 16px; font-weight: bold; padding: 10px; background-color: rgba(0, 0, 0, 0.5); border-radius: 5px; }");
    m_loadingLabel->setAlignment(Qt::AlignCenter);
    overlayLayout->addWidget(m_loadingLabel, 0, Qt::AlignCenter);

    m_loadingProgressLabel = new QLabel("0%");
    m_loadingProgressLabel->setStyleSheet("QLabel { color: #3498db; font-size: 24px; font-weight: bold; padding: 5px; }");
    m_loadingProgressLabel->setAlignment(Qt::AlignCenter);
    overlayLayout->addWidget(m_loadingProgressLabel, 0, Qt::AlignCenter);

    m_loadingProgressBar = new QProgressBar();
    m_loadingProgressBar->setStyleSheet("QProgressBar { border: 2px solid #3498db; border-radius: 5px; text-align: center; background-color: rgba(0, 0, 0, 0.5); color: white; } QProgressBar::chunk { background-color: #3498db; border-radius: 3px; }");
    m_loadingProgressBar->setMinimum(0);
    m_loadingProgressBar->setMaximum(100);
    overlayLayout->addWidget(m_loadingProgressBar, 0, Qt::AlignCenter);
}

void MainWindow::showLoadingOverlay(const QString& message)
{
    m_loadingActive = true;
    m_loadingLabel->setText(message);
    m_loadingProgressLabel->setText("0%");
    if (m_loadingProgressBar) m_loadingProgressBar->setValue(0);
    m_loadingOverlay->resize(size());
    m_loadingOverlay->raise();
    m_loadingOverlay->show();
    qApp->processEvents();
}

void MainWindow::hideLoadingOverlay()
{
    if (!m_loadingActive) return;
    m_loadingActive = false;
    m_loadingOverlay->hide();
    qApp->processEvents();
}

void MainWindow::updateLoadingProgress(int percent, const QString& message)
{
    if (!m_loadingActive) return;
    m_loadingLabel->setText(message);
    m_loadingProgressLabel->setText(QString("%1%").arg(percent));
    if (m_loadingProgressBar) m_loadingProgressBar->setValue(percent);
    qApp->processEvents();
}

void MainWindow::switchToPage(int index)
{
    btnDataView->setChecked(index == 0);
    btnSettings->setChecked(index == 1);
    stackedWidget->setCurrentIndex(index);
}

void MainWindow::loadDefaultFile()
{
    QStringList possiblePaths = {
        "/home/kc/data_recording/DRF_43.TXT",
        QDir::homePath() + "/data_recording/DRF_43.TXT",
        QDir::homePath() + "/DRF_43.TXT",
        QApplication::applicationDirPath() + "/DRF_43.TXT"
    };

    for (const QString& path : possiblePaths) {
        if (QFile::exists(path)) {
            currentFile = path;
            loadFile();
            return;
        }
    }
    statusBar()->showMessage("No default file found. Please use 'Browse File' to select a data file.", 5000);
    setupEmptyTable();
}

void MainWindow::setupEmptyTable()
{
    if (frozenTableWidget) frozenTableWidget->setRowCount(0);
    if (dataTableWidget) dataTableWidget->setRowCount(0);
    statusBar()->showMessage("Ready - Load a file using 'Browse File' button", 3000);
}

void MainWindow::applyInitialTheme()
{
    if (m_currentTheme == "Dark") {
        darkThemeRadio->setChecked(true);
        applyDarkTheme();
    } else {
        lightThemeRadio->setChecked(true);
        applyLightTheme();
    }
    updateColorButtons();
}

QString MainWindow::fixTimeFormat(const QString& time)
{
    QString fixed = time;
    fixed.replace('.', ':');
    fixed.replace('-', ':');
    fixed.replace('/', ':');
    return fixed;
}

void MainWindow::loadFile()
{
    if(!QFile::exists(currentFile)) {
        statusBar()->showMessage("File not found: " + currentFile);
        setupEmptyTable();
        return;
    }

    QFile file(currentFile);
    if(!file.open(QIODevice::ReadOnly)) {
        statusBar()->showMessage("Cannot open file");
        return;
    }

    showLoadingOverlay("Preparing to load file...");

    QByteArray firstLine = file.readLine();
    file.close();

    QString firstLineStr = QString::fromUtf8(firstLine).trimmed();
    bool hasHeaders = (firstLineStr.contains("DATE", Qt::CaseInsensitive) &&
                       firstLineStr.contains("TIME", Qt::CaseInsensitive) &&
                       firstLineStr.contains("SPEED", Qt::CaseInsensitive));

    if(!file.open(QIODevice::ReadOnly)) {
        hideLoadingOverlay();
        statusBar()->showMessage("Cannot open file");
        return;
    }

    updateLoadingProgress(5, "Reading file data...");

    QByteArray data = file.readAll();
    file.close();

    updateLoadingProgress(15, "Processing file content...");

    QString text = QString::fromUtf8(data);
    text.replace("\r", "");
    text.remove(QRegularExpression("===== Page \\d+ =====\\n?"));

    updateLoadingProgress(25, "Parsing data lines...");

    QStringList lines = text.split("\n", Qt::SkipEmptyParts);

    updateLoadingProgress(30, "Setting up tables...");

    clearTables();
    clearAllFilters();

    int startLine = hasHeaders ? 1 : 0;

    updateLoadingProgress(35, "Loading data...");

    m_maxRowsLoaded = 0;
    int totalLines = lines.size();
    int linesToProcess = totalLines - startLine;

    if (linesToProcess <= 0) {
        hideLoadingOverlay();
        statusBar()->showMessage("No data rows found in file");
        setupEmptyTable();
        return;
    }

    frozenTableWidget->setUpdatesEnabled(false);
    dataTableWidget->setUpdatesEnabled(false);
    frozenTableWidget->setSortingEnabled(false);
    dataTableWidget->setSortingEnabled(false);

    for(int chunkStart = startLine; chunkStart < totalLines && m_maxRowsLoaded < MAX_ROWS; chunkStart += CHUNK_SIZE) {
        int chunkEnd = qMin(chunkStart + CHUNK_SIZE, totalLines);

        for(int i = chunkStart; i < chunkEnd && m_maxRowsLoaded < MAX_ROWS; i++) {
            QString line = lines[i].trimmed();
            if(line.isEmpty()) continue;

            QStringList values = line.split(",", Qt::SkipEmptyParts);
            if(values.size() < 2) continue;

            frozenTableWidget->insertRow(m_maxRowsLoaded);
            dataTableWidget->insertRow(m_maxRowsLoaded);

            QString fixedTime = fixTimeFormat(values[1].trimmed());
            addFrozenRow(m_maxRowsLoaded, values[0].trimmed(), fixedTime);

            bool isDarkTheme = (m_currentTheme == "Dark");
            for(int col = 0; col < 43; col++) {
                QString value = (col + 2 < values.size()) ? values[col + 2].trimmed() : "";
                QTableWidgetItem *item = new QTableWidgetItem(value);
                item->setForeground(isDarkTheme ? QBrush(QColor(255, 255, 255)) : QBrush(QColor(0, 0, 0)));
                item->setToolTip(value);
                dataTableWidget->setItem(m_maxRowsLoaded, col, item);
            }
            m_maxRowsLoaded++;
        }

        int percent = 35 + ((m_maxRowsLoaded * 60) / qMax(1, linesToProcess));
        percent = qMin(percent, 95);
        updateLoadingProgress(percent, QString("Loading data... (%1 rows loaded)").arg(m_maxRowsLoaded));
        qApp->processEvents();
    }

    updateLoadingProgress(95, "Finalizing table setup...");

    frozenTableWidget->resizeColumnsToContents();
    dataTableWidget->resizeColumnsToContents();

    if (frozenTableWidget->columnWidth(0) < FROZEN_COLUMN_WIDTH_DATE)
        frozenTableWidget->setColumnWidth(0, FROZEN_COLUMN_WIDTH_DATE);
    if (frozenTableWidget->columnWidth(1) < FROZEN_COLUMN_WIDTH_TIME)
        frozenTableWidget->setColumnWidth(1, FROZEN_COLUMN_WIDTH_TIME);

    for(int i = 0; i < dataTableWidget->columnCount(); i++) {
        int width = dataTableWidget->columnWidth(i);
        if (width < 80) dataTableWidget->setColumnWidth(i, 80);
        if (width > 150) dataTableWidget->setColumnWidth(i, 150);
    }

    frozenTableWidget->setFixedWidth(frozenTableWidget->columnWidth(0) + frozenTableWidget->columnWidth(1) + 10);
    syncRowHeights();

    frozenTableWidget->setUpdatesEnabled(true);
    dataTableWidget->setUpdatesEnabled(true);
    frozenTableWidget->setSortingEnabled(true);
    dataTableWidget->setSortingEnabled(true);

    rebuildFilterMaps();
    refreshFrozenTableVisibility();

    updateLoadingProgress(100, "Loading complete!");
    hideLoadingOverlay();

    statusBar()->showMessage(QString("Loaded %1 rows from: %2").arg(m_maxRowsLoaded).arg(currentFile));
}

void MainWindow::addFrozenRow(int row, const QString& date, const QString& time)
{
    QTableWidgetItem *dateItem = createTableItem(date);
    QTableWidgetItem *timeItem = createTableItem(time);
    frozenTableWidget->setItem(row, 0, dateItem);
    frozenTableWidget->setItem(row, 1, timeItem);
}

QTableWidgetItem* MainWindow::createTableItem(const QString& text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    item->setForeground((m_currentTheme == "Dark") ? QBrush(QColor(255, 255, 255)) : QBrush(QColor(0, 0, 0)));
    item->setToolTip(text);
    return item;
}

void MainWindow::clearTables()
{
    if (frozenTableWidget) {
        frozenTableWidget->clearContents();
        frozenTableWidget->setRowCount(0);
    }
    if (dataTableWidget) {
        dataTableWidget->clearContents();
        dataTableWidget->setRowCount(0);
    }
    m_columnFilters.clear();
    m_columnUniqueValues.clear();
    m_searchHighlightedItems.clear();
}

void MainWindow::syncRowHeights()
{
    if (!frozenTableWidget || !dataTableWidget) return;
    int rowCount = frozenTableWidget->rowCount();
    for(int row = 0; row < rowCount; row++) {
        int frozenHeight = frozenTableWidget->rowHeight(row);
        int dataHeight = dataTableWidget->rowHeight(row);
        if(frozenHeight != dataHeight) {
            int maxHeight = qMax(frozenHeight, dataHeight);
            if(maxHeight < 20) maxHeight = 20;
            frozenTableWidget->setRowHeight(row, maxHeight);
            dataTableWidget->setRowHeight(row, maxHeight);
        }
    }
}

void MainWindow::syncFrozenTableAfterSort()
{
    if (!frozenTableWidget || !dataTableWidget) return;
    QVector<QPair<QString, QString>> frozenData;
    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        QString date = frozenTableWidget->item(row, 0) ? frozenTableWidget->item(row, 0)->text() : "";
        QString time = frozenTableWidget->item(row, 1) ? frozenTableWidget->item(row, 1)->text() : "";
        frozenData.append(qMakePair(date, time));
    }
    frozenTableWidget->clearContents();
    for (int row = 0; row < frozenData.size(); row++) {
        frozenTableWidget->setItem(row, 0, createTableItem(frozenData[row].first));
        frozenTableWidget->setItem(row, 1, createTableItem(frozenData[row].second));
    }
    syncRowHeights();
}

void MainWindow::refreshFrozenTableVisibility()
{
    if (!frozenTableWidget || !dataTableWidget) return;
    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        bool isHidden = dataTableWidget->isRowHidden(row);
        if (frozenTableWidget->isRowHidden(row) != isHidden) {
            frozenTableWidget->setRowHidden(row, isHidden);
        }
    }
}

void MainWindow::showLoginDialog()
{
    LoginWindow loginDialog(this);
    loginDialog.updateCredentials(m_adminUsername, m_adminPassword, m_userUsername, m_userPassword);
    loginDialog.setModal(true);

    // Qt 6 way to center the dialog on screen
    QRect screenGeometry = QGuiApplication::primaryScreen()->geometry();
    int x = (screenGeometry.width() - loginDialog.width()) / 2;
    int y = (screenGeometry.height() - loginDialog.height()) / 2;
    loginDialog.move(x, y);

    if (loginDialog.exec() == QDialog::Accepted) {
        m_currentRole = loginDialog.loggedInRole();
        updateAccessRights();

        QString roleStr = (m_currentRole == "Admin") ? "Administrator" : "User";
        QMessageBox::information(this, "Login Success",
                                 QString("Welcome to TrainTech Electronics!\n\nYou are now logged in as %1.\n%2")
                                     .arg(roleStr)
                                     .arg(m_currentRole == "Admin" ? "You can change credentials in the Settings page." : "You have read-only access."));
        statusBar()->showMessage(QString("Logged in as %1").arg(roleStr), 5000);
    } else {
        m_currentRole = "Guest";
        updateAccessRights();
    }
}

void MainWindow::updateAccessRights()
{
    bool isAdmin = (m_currentRole == "Admin");

    if (adminUsernameEdit) {
        adminUsernameEdit->setReadOnly(!isAdmin);
        adminPasswordEdit->setReadOnly(!isAdmin);
        userUsernameEdit->setReadOnly(!isAdmin);
        userPasswordEdit->setReadOnly(!isAdmin);

        if (isAdmin) {
            adminUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 2px solid #28a745; border-radius: 4px; background-color: #f0fff0; } QLineEdit:focus { border-color: #28a745; }");
            adminPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 2px solid #28a745; border-radius: 4px; background-color: #f0fff0; } QLineEdit:focus { border-color: #28a745; }");
            userUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 2px solid #28a745; border-radius: 4px; background-color: #f0fff0; } QLineEdit:focus { border-color: #28a745; }");
            userPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 2px solid #28a745; border-radius: 4px; background-color: #f0fff0; } QLineEdit:focus { border-color: #28a745; }");

            adminUsernameEdit->setPlaceholderText("Admin Username (editable)");
            adminPasswordEdit->setPlaceholderText("Admin Password (editable)");
            userUsernameEdit->setPlaceholderText("User Username (editable)");
            userPasswordEdit->setPlaceholderText("User Password (editable)");
        } else {
            adminUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; background-color: #f5f5f5; }");
            adminPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; background-color: #f5f5f5; }");
            userUsernameEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; background-color: #f5f5f5; }");
            userPasswordEdit->setStyleSheet("QLineEdit { padding: 5px; border: 1px solid #ccc; border-radius: 4px; background-color: #f5f5f5; }");

            adminUsernameEdit->setPlaceholderText("Admin Username (login required to edit)");
            adminPasswordEdit->setPlaceholderText("Admin Password (login required to edit)");
            userUsernameEdit->setPlaceholderText("User Username (login required to edit)");
            userPasswordEdit->setPlaceholderText("User Password (login required to edit)");
        }
    }
    if (saveSettingsButton) saveSettingsButton->setEnabled(isAdmin);
    if (m_resetSettingsButton) m_resetSettingsButton->setEnabled(isAdmin);

    if (isAdmin) {
        loginButton->setText("👤 Admin Mode");
        loginButton->setStyleSheet("QPushButton { background-color: #dc3545; color: white; padding: 4px; border-radius: 4px; font-weight: bold; } QPushButton:hover { background-color: #c82333; }");
        loginButton->setToolTip("Logged in as Admin");
    } else if (m_currentRole == "User") {
        loginButton->setText("👤 User Mode");
        loginButton->setStyleSheet("QPushButton { background-color: #ffc107; color: black; padding: 4px; border-radius: 4px; font-weight: bold; } QPushButton:hover { background-color: #e0a800; }");
        loginButton->setToolTip("Logged in as User - Limited access");
    } else {
        loginButton->setText("🔐 Login (Guest)");
        loginButton->setStyleSheet("QPushButton { background-color: #28a745; color: white; padding: 4px; border-radius: 4px; font-weight: bold; } QPushButton:hover { background-color: #218838; }");
        loginButton->setToolTip("Click to login as Admin or User");
    }
}

void MainWindow::loadSettings()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    if(QFile::exists(settingsFile)) {
        QFile file(settingsFile);
        if(file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            QTextStream in(&file);
            while(!in.atEnd()) {
                QString line = in.readLine();
                if(line.startsWith("AdminUsername="))
                    m_adminUsername = line.mid(14).trimmed();
                else if(line.startsWith("AdminPassword="))
                    m_adminPassword = line.mid(14).trimmed();
                else if(line.startsWith("UserUsername="))
                    m_userUsername = line.mid(13).trimmed();
                else if(line.startsWith("UserPassword="))
                    m_userPassword = line.mid(13).trimmed();
                else if(line.startsWith("Theme="))
                    m_currentTheme = line.mid(6).trimmed();
                else if(line.startsWith("HighlightingEnabled="))
                    m_highlightingEnabled = (line.mid(20).trimmed() == "true");
                else if(line.startsWith("RowHighlightColor="))
                    m_rowHighlightColor = QColor(line.mid(18).trimmed());
                else if(line.startsWith("ColumnHighlightColor="))
                    m_columnHighlightColor = QColor(line.mid(21).trimmed());
            }
            file.close();
        }
    }

    if (adminUsernameEdit) adminUsernameEdit->setText(m_adminUsername);
    if (adminPasswordEdit) adminPasswordEdit->setText(m_adminPassword);
    if (userUsernameEdit) userUsernameEdit->setText(m_userUsername);
    if (userPasswordEdit) userPasswordEdit->setText(m_userPassword);
}

void MainWindow::saveSettings()
{
    if (m_currentRole != "Admin") {
        QMessageBox::warning(this, "Access Denied", "Only administrators can save settings.\n\nPlease login as Admin to edit credentials.");
        return;
    }

    QString newAdminUsername = adminUsernameEdit->text().trimmed();
    QString newAdminPassword = adminPasswordEdit->text().trimmed();
    QString newUserUsername = userUsernameEdit->text().trimmed();
    QString newUserPassword = userPasswordEdit->text().trimmed();

    if (newAdminUsername.isEmpty() || newAdminPassword.isEmpty() ||
        newUserUsername.isEmpty() || newUserPassword.isEmpty()) {
        QMessageBox::warning(this, "Invalid Input", "All username and password fields must be filled.");
        return;
    }

    if (newAdminPassword.length() < 4) {
        QMessageBox::warning(this, "Weak Password", "Admin password must be at least 4 characters long.");
        return;
    }

    if (newUserPassword.length() < 4) {
        QMessageBox::warning(this, "Weak Password", "User password must be at least 4 characters long.");
        return;
    }

    m_adminUsername = newAdminUsername;
    m_adminPassword = newAdminPassword;
    m_userUsername = newUserUsername;
    m_userPassword = newUserPassword;
    m_currentTheme = darkThemeRadio->isChecked() ? "Dark" : "Light";
    m_highlightingEnabled = enableHighlightingCheckbox->isChecked();

    if (saveSettingsInternal()) {
        QMessageBox::information(this, "Settings Saved", "Settings have been saved successfully!");
        statusBar()->showMessage("Credentials saved successfully!", 3000);
    } else {
        QMessageBox::critical(this, "Error", "Failed to save settings file!");
    }
}

bool MainWindow::saveSettingsInternal()
{
    QString settingsFile = QApplication::applicationDirPath() + "/settings.ini";
    QFile file(settingsFile);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) return false;

    QTextStream out(&file);
    out << "[Login Credentials]\n";
    out << "AdminUsername=" << m_adminUsername << "\n";
    out << "AdminPassword=" << m_adminPassword << "\n";
    out << "UserUsername=" << m_userUsername << "\n";
    out << "UserPassword=" << m_userPassword << "\n";
    out << "Theme=" << m_currentTheme << "\n";
    out << "\n[Highlighting Settings]\n";
    out << "HighlightingEnabled=" << (m_highlightingEnabled ? "true" : "false") << "\n";
    out << "RowHighlightColor=" << m_rowHighlightColor.name() << "\n";
    out << "ColumnHighlightColor=" << m_columnHighlightColor.name() << "\n";
    out << "\n[End of Settings]\n";
    file.close();
    return true;
}

void MainWindow::resetSettings()
{
    if (m_currentRole != "Admin") {
        QMessageBox::warning(this, "Access Denied", "Only administrators can reset settings.");
        return;
    }

    QMessageBox::StandardButton reply = QMessageBox::question(this, "Reset Settings",
                                                              "Are you sure you want to reset all settings to default?\n\n"
                                                              "This will change:\n"
                                                              "• Admin credentials back to: admin / 1234\n"
                                                              "• User credentials back to: user / user123\n"
                                                              "• Theme back to Light\n"
                                                              "• Highlighting settings back to default\n\n"
                                                              "You will need to login again with the default credentials.",
                                                              QMessageBox::Yes | QMessageBox::No);

    if (reply == QMessageBox::Yes) {
        m_adminUsername = "admin";
        m_adminPassword = "1234";
        m_userUsername = "user";
        m_userPassword = "user123";
        m_rowHighlightColor = QColor(173, 216, 230);
        m_columnHighlightColor = QColor(100, 149, 237);
        m_highlightingEnabled = true;
        m_currentTheme = "Light";

        if (adminUsernameEdit) {
            adminUsernameEdit->setText(m_adminUsername);
            adminPasswordEdit->setText(m_adminPassword);
            userUsernameEdit->setText(m_userUsername);
            userPasswordEdit->setText(m_userPassword);
        }
        if (enableHighlightingCheckbox) enableHighlightingCheckbox->setChecked(true);
        if (lightThemeRadio) lightThemeRadio->setChecked(true);

        saveSettingsInternal();
        updateColorButtons();

        QMessageBox::information(this, "Reset Complete", "Settings have been reset to default values.\n\nDefault Admin: admin / 1234\nDefault User: user / user123");
        statusBar()->showMessage("Settings reset to default!", 3000);
    }
}

void MainWindow::applyTheme(bool checked)
{
    Q_UNUSED(checked);
    if (darkThemeRadio->isChecked()) {
        m_currentTheme = "Dark";
        applyDarkTheme();
    } else {
        m_currentTheme = "Light";
        applyLightTheme();
    }

    if (dataTableWidget && dataTableWidget->rowCount() > 0) {
        bool isDarkTheme = (m_currentTheme == "Dark");
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            for (int col = 0; col < dataTableWidget->columnCount(); col++) {
                QTableWidgetItem *item = dataTableWidget->item(row, col);
                if (item) item->setForeground(isDarkTheme ? QBrush(QColor(255, 255, 255)) : QBrush(QColor(0, 0, 0)));
            }
        }
        for (int row = 0; row < frozenTableWidget->rowCount(); row++) {
            for (int col = 0; col < frozenTableWidget->columnCount(); col++) {
                QTableWidgetItem *item = frozenTableWidget->item(row, col);
                if (item) item->setForeground(isDarkTheme ? QBrush(QColor(255, 255, 255)) : QBrush(QColor(0, 0, 0)));
            }
        }

        // Reapply current highlight if any
        if (m_highlightingEnabled && m_currentHighlightedRow >= 0) {
            if (m_currentHighlightedColumn >= 0) {
                applyCrossHighlight(m_currentHighlightedRow, m_currentHighlightedColumn);
            } else if (m_currentHighlightedRow >= 0) {
                onFrozenTableCellClicked(m_currentHighlightedRow, 0);
            }
        }
    }
}

void MainWindow::applyLightTheme()
{
    qApp->setStyleSheet(
        "QMainWindow { background-color: #f4f6f8; }"
        "QTableWidget { background-color: white; color: #000000; gridline-color: #dcdcdc; alternate-background-color: #f5f5f5; }"
        "QHeaderView::section { background-color: #e9ecef; color: #000000; padding: 8px; font-weight: bold; border: 1px solid #dcdcdc; }"
        "QPushButton { background-color: #0078d7; color: white; padding: 6px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #0095ff; }"
        "QLineEdit { color: #000000; background-color: white; padding: 8px; border: 1px solid #cfcfcf; border-radius: 5px; }"
        "QLabel { color: #000000; }"
        "QStatusBar { color: #000000; background-color: #e9ecef; }"
        "QGroupBox { color: #000000; border: 2px solid #cccccc; border-radius: 8px; margin-top: 8px; padding-top: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        );
}

void MainWindow::applyDarkTheme()
{
    qApp->setStyleSheet(
        "QMainWindow { background-color: #2b2b2b; }"
        "QTableWidget { background-color: #3c3c3c; color: #ffffff; gridline-color: #555555; alternate-background-color: #454545; }"
        "QHeaderView::section { background-color: #4a4a4a; color: #ffffff; padding: 8px; font-weight: bold; border: 1px solid #555555; }"
        "QPushButton { background-color: #0d7377; color: white; padding: 6px; border-radius: 5px; }"
        "QPushButton:hover { background-color: #14a085; }"
        "QLineEdit { color: #ffffff; background-color: #4a4a4a; padding: 8px; border: 1px solid #666666; border-radius: 5px; }"
        "QLabel { color: #ffffff; }"
        "QStatusBar { color: #ffffff; background-color: #4a4a4a; }"
        "QGroupBox { color: #ffffff; border: 2px solid #666666; border-radius: 8px; margin-top: 8px; padding-top: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 10px; padding: 0 5px 0 5px; }"
        );
}

void MainWindow::findText()
{
    QString search = searchEdit->text().trimmed();
    if(search.isEmpty()) return;

    bool found = false;
    int foundCount = 0;

    clearSearchHighlights();

    for(int row = 0; row < frozenTableWidget->rowCount(); row++) {
        for(int col = 0; col < frozenTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = frozenTableWidget->item(row, col);
            if(item && item->text().contains(search, Qt::CaseInsensitive)) {
                item->setBackground(QColor(255, 220, 120));
                m_searchHighlightedItems.insert(item);
                if (!found) frozenTableWidget->scrollToItem(item);
                found = true;
                foundCount++;
            }
        }
    }

    for(int row = 0; row < dataTableWidget->rowCount(); row++) {
        for(int col = 0; col < dataTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = dataTableWidget->item(row, col);
            if(item && item->text().contains(search, Qt::CaseInsensitive)) {
                item->setBackground(QColor(255, 220, 120));
                m_searchHighlightedItems.insert(item);
                if (!found) dataTableWidget->scrollToItem(item);
                found = true;
                foundCount++;
            }
        }
    }

    if(!found)
        QMessageBox::information(this, "Search", "No matching data found");
    else
        statusBar()->showMessage(QString("Found %1 matching cells").arg(foundCount));
}

void MainWindow::clearSearchHighlights()
{
    for (QTableWidgetItem* item : m_searchHighlightedItems) {
        if (item) item->setBackground(QBrush());
    }
    m_searchHighlightedItems.clear();
}

void MainWindow::clearSearch()
{
    searchEdit->clear();
    clearSearchHighlights();
    statusBar()->showMessage("Search cleared");
}

void MainWindow::clearRowColumnHighlights()
{
    // Clear frozen table highlights
    for (int r = 0; r < frozenTableWidget->rowCount(); r++) {
        for (int c = 0; c < frozenTableWidget->columnCount(); c++) {
            QTableWidgetItem *item = frozenTableWidget->item(r, c);
            if (item) {
                item->setBackground(QBrush());
                item->setForeground((m_currentTheme == "Dark") ? QBrush(Qt::white) : QBrush(Qt::black));
            }
        }
    }

    // Clear data table highlights
    for (int r = 0; r < dataTableWidget->rowCount(); r++) {
        for (int c = 0; c < dataTableWidget->columnCount(); c++) {
            QTableWidgetItem *item = dataTableWidget->item(r, c);
            if (item) {
                item->setBackground(QBrush());
                item->setForeground((m_currentTheme == "Dark") ? QBrush(Qt::white) : QBrush(Qt::black));
            }
        }
    }

    m_currentHighlightedRow = -1;
    m_currentHighlightedColumn = -1;

    // Force repaint
    dataTableWidget->viewport()->update();
    frozenTableWidget->viewport()->update();
}

void MainWindow::applyCrossHighlight(int row, int column)
{
    if (!m_highlightingEnabled) return;

    qDebug() << "Applying highlight - Row:" << row << "Column:" << column;
    qDebug() << "Row color:" << m_rowHighlightColor.name();
    qDebug() << "Column color:" << m_columnHighlightColor.name();

    // First, clear all existing highlights
    clearRowColumnHighlights();

    // Highlight entire row
    QBrush rowBrush(m_rowHighlightColor);
    for (int c = 0; c < dataTableWidget->columnCount(); c++) {
        QTableWidgetItem *item = dataTableWidget->item(row, c);
        if (item) {
            item->setBackground(rowBrush);
            item->setForeground((m_currentTheme == "Dark") ? Qt::white : Qt::black);
        }
    }

    for (int c = 0; c < frozenTableWidget->columnCount(); c++) {
        QTableWidgetItem *item = frozenTableWidget->item(row, c);
        if (item) {
            item->setBackground(rowBrush);
            item->setForeground((m_currentTheme == "Dark") ? Qt::white : Qt::black);
        }
    }

    // Highlight column (other rows)
    QBrush columnBrush(m_columnHighlightColor);
    for (int r = 0; r < dataTableWidget->rowCount(); r++) {
        if (r != row) {
            QTableWidgetItem *item = dataTableWidget->item(r, column);
            if (item) {
                item->setBackground(columnBrush);
                item->setForeground((m_currentTheme == "Dark") ? Qt::white : Qt::black);
            }
        }
    }

    // Highlight the intersection cell with a distinct color
    QTableWidgetItem *cell = dataTableWidget->item(row, column);
    if (cell) {
        cell->setBackground(QBrush(QColor(0, 112, 192)));  // Deep blue
        cell->setForeground(Qt::white);
    }

    m_currentHighlightedRow = row;
    m_currentHighlightedColumn = column;

    // Force repaint
    dataTableWidget->viewport()->update();
    frozenTableWidget->viewport()->update();
}

void MainWindow::onTableCellClicked(int row, int column)
{
    qDebug() << "Cell clicked - Row:" << row << "Column:" << column;
    applyCrossHighlight(row, column);
    QString columnName = dataTableWidget->horizontalHeaderItem(column) ?
                             dataTableWidget->horizontalHeaderItem(column)->text() : "";
    statusBar()->showMessage(QString("Row %1, Column %2 - %3").arg(row + 1).arg(column + 1).arg(columnName), 3000);
}

void MainWindow::onFrozenTableCellClicked(int row, int column)
{
    qDebug() << "Frozen cell clicked - Row:" << row << "Column:" << column;
    if (!m_highlightingEnabled) return;

    clearRowColumnHighlights();

    // Highlight entire row only
    QBrush rowBrush(m_rowHighlightColor);
    for (int c = 0; c < dataTableWidget->columnCount(); c++) {
        QTableWidgetItem *item = dataTableWidget->item(row, c);
        if (item) {
            item->setBackground(rowBrush);
            item->setForeground((m_currentTheme == "Dark") ? Qt::white : Qt::black);
        }
    }

    for (int c = 0; c < frozenTableWidget->columnCount(); c++) {
        QTableWidgetItem *item = frozenTableWidget->item(row, c);
        if (item) {
            item->setBackground(rowBrush);
            item->setForeground((m_currentTheme == "Dark") ? Qt::white : Qt::black);
        }
    }

    m_currentHighlightedRow = row;
    m_currentHighlightedColumn = -1;

    QString date = frozenTableWidget->item(row, 0) ? frozenTableWidget->item(row, 0)->text() : "";
    QString time = frozenTableWidget->item(row, 1) ? frozenTableWidget->item(row, 1)->text() : "";
    statusBar()->showMessage(QString("Row %1 selected - Date: %2, Time: %3").arg(row + 1).arg(date).arg(time), 3000);

    // Force repaint
    dataTableWidget->viewport()->update();
    frozenTableWidget->viewport()->update();
}

void MainWindow::toggleHighlighting(bool enabled)
{
    m_highlightingEnabled = enabled;
    if (!enabled) {
        clearRowColumnHighlights();
    }
    statusBar()->showMessage(QString("Row/Column highlighting %1").arg(enabled ? "enabled" : "disabled"), 2000);
}

void MainWindow::changeRowHighlightColor()
{
    QColor color = QColorDialog::getColor(m_rowHighlightColor, this, "Select Row Highlight Color");
    if (color.isValid()) {
        m_rowHighlightColor = color;
        updateColorButtons();
        if (m_currentHighlightedRow >= 0 && m_highlightingEnabled) {
            if (m_currentHighlightedColumn >= 0)
                applyCrossHighlight(m_currentHighlightedRow, m_currentHighlightedColumn);
            else
                onFrozenTableCellClicked(m_currentHighlightedRow, 0);
        }
    }
}

void MainWindow::changeColumnHighlightColor()
{
    QColor color = QColorDialog::getColor(m_columnHighlightColor, this, "Select Column Highlight Color");
    if (color.isValid()) {
        m_columnHighlightColor = color;
        updateColorButtons();
        if (m_currentHighlightedRow >= 0 && m_currentHighlightedColumn >= 0 && m_highlightingEnabled) {
            applyCrossHighlight(m_currentHighlightedRow, m_currentHighlightedColumn);
        }
    }
}

void MainWindow::showColumnFilterMenu(int column)
{
    if (!dataTableWidget->horizontalHeaderItem(column)) return;

    QSet<QString> uniqueValues;
    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        QTableWidgetItem* item = dataTableWidget->item(row, column);
        if (item && !item->text().isEmpty()) {
            uniqueValues.insert(item->text());
        }
    }

    QStringList sortedValues = uniqueValues.values();
    bool allNumeric = true;
    for (const QString& val : sortedValues) {
        bool ok;
        val.toDouble(&ok);
        if (!ok) { allNumeric = false; break; }
    }

    if (allNumeric) {
        std::sort(sortedValues.begin(), sortedValues.end(),
                  [](const QString& a, const QString& b) {
                      return a.toDouble() < b.toDouble();
                  });
    } else {
        sortedValues.sort(Qt::CaseInsensitive);
    }

    QDialog filterDialog(this);
    filterDialog.setWindowTitle(QString("Filter: %1").arg(dataTableWidget->horizontalHeaderItem(column)->text()));
    filterDialog.setModal(true);
    filterDialog.setMinimumWidth(350);
    filterDialog.setMinimumHeight(400);

    QVBoxLayout* layout = new QVBoxLayout(&filterDialog);

    QGroupBox* quickFilterGroup = new QGroupBox("Quick Filters");
    QVBoxLayout* quickLayout = new QVBoxLayout(quickFilterGroup);

    QPushButton* allButton = new QPushButton("✓ Select All");
    QPushButton* clearButton = new QPushButton("✗ Clear All");
    QPushButton* invertButton = new QPushButton("⟳ Invert Selection");
    QPushButton* numberRangeButton = new QPushButton("🔢 Number Range Filter...");
    QPushButton* textFilterButton = new QPushButton("🔤 Text Filter...");

    QGridLayout* buttonGrid = new QGridLayout();
    buttonGrid->addWidget(allButton, 0, 0);
    buttonGrid->addWidget(clearButton, 0, 1);
    buttonGrid->addWidget(invertButton, 0, 2);
    buttonGrid->addWidget(numberRangeButton, 1, 0, 1, 2);
    buttonGrid->addWidget(textFilterButton, 1, 2);

    quickLayout->addLayout(buttonGrid);
    layout->addWidget(quickFilterGroup);

    QLineEdit* searchBox = new QLineEdit();
    searchBox->setPlaceholderText("🔍 Search values...");
    searchBox->setClearButtonEnabled(true);
    layout->addWidget(searchBox);

    QListWidget* valueList = new QListWidget();
    valueList->setSelectionMode(QAbstractItemView::NoSelection);
    valueList->setAlternatingRowColors(true);

    QSet<QString> currentSelection;
    if (m_columnFilters.contains(column)) {
        QStringList filterList = m_columnFilters[column];
        for (const QString& val : filterList) {
            currentSelection.insert(val);
        }
    } else {
        currentSelection = uniqueValues;
    }

    QHash<QString, QCheckBox*> checkBoxMap;
    for (const QString& value : sortedValues) {
        QCheckBox* checkBox = new QCheckBox(value);
        checkBox->setChecked(currentSelection.contains(value));
        QListWidgetItem* item = new QListWidgetItem(valueList);
        valueList->setItemWidget(item, checkBox);
        checkBoxMap[value] = checkBox;
    }
    layout->addWidget(valueList);

    QLabel* statusLabel = new QLabel(QString("Showing %1 of %2 values").arg(sortedValues.size()).arg(sortedValues.size()));
    layout->addWidget(statusLabel);

    connect(searchBox, &QLineEdit::textChanged, [valueList, statusLabel, sortedValues](const QString& text) {
        int visibleCount = 0;
        for (int i = 0; i < valueList->count(); i++) {
            QListWidgetItem* item = valueList->item(i);
            QCheckBox* checkBox = qobject_cast<QCheckBox*>(valueList->itemWidget(item));
            if (checkBox) {
                bool matches = text.isEmpty() || checkBox->text().contains(text, Qt::CaseInsensitive);
                item->setHidden(!matches);
                if (matches) visibleCount++;
            }
        }
        statusLabel->setText(QString("Showing %1 of %2 values").arg(visibleCount).arg(sortedValues.size()));
    });

    connect(allButton, &QPushButton::clicked, [&checkBoxMap]() {
        for (QCheckBox* box : checkBoxMap) box->setChecked(true);
    });

    connect(clearButton, &QPushButton::clicked, [&checkBoxMap]() {
        for (QCheckBox* box : checkBoxMap) box->setChecked(false);
    });

    connect(invertButton, &QPushButton::clicked, [&checkBoxMap]() {
        for (QCheckBox* box : checkBoxMap) box->setChecked(!box->isChecked());
    });

    if (allNumeric) {
        connect(numberRangeButton, &QPushButton::clicked, [&]() {
            bool ok;
            double minVal = QInputDialog::getDouble(&filterDialog, "Minimum Value", "Enter minimum value:", -999999, -9999999, 9999999, 2, &ok);
            if (!ok) return;
            double maxVal = QInputDialog::getDouble(&filterDialog, "Maximum Value", "Enter maximum value:", 999999, -9999999, 9999999, 2, &ok);
            if (!ok) return;
            for (QCheckBox* box : checkBoxMap) {
                double val = box->text().toDouble();
                box->setChecked(val >= minVal && val <= maxVal);
            }
        });
    } else {
        numberRangeButton->setEnabled(false);
    }

    connect(textFilterButton, &QPushButton::clicked, [&]() {
        bool ok;
        QString filterText = QInputDialog::getText(&filterDialog, "Text Filter",
                                                   "Enter text to filter (supports * for wildcard):\nExample: *error* or start* or *end",
                                                   QLineEdit::Normal, "", &ok);
        if (!ok || filterText.isEmpty()) return;

        QRegularExpression regex(QRegularExpression::wildcardToRegularExpression(filterText),
                                 QRegularExpression::CaseInsensitiveOption);

        int matchedCount = 0;
        for (QCheckBox* box : checkBoxMap) {
            bool matches = regex.match(box->text()).hasMatch();
            box->setChecked(matches);
            if (matches) matchedCount++;
        }
        statusBar()->showMessage(QString("Text filter matched %1 values").arg(matchedCount), 2000);
    });

    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply);
    buttonBox->button(QDialogButtonBox::Apply)->setText("Apply");

    QPushButton* clearFilterButton = new QPushButton("Clear Filter");
    buttonBox->addButton(clearFilterButton, QDialogButtonBox::ActionRole);

    layout->addWidget(buttonBox);

    auto getSelectedValues = [&checkBoxMap]() -> QStringList {
        QStringList selected;
        for (auto it = checkBoxMap.begin(); it != checkBoxMap.end(); ++it) {
            if (it.value()->isChecked()) selected.append(it.key());
        }
        return selected;
    };

    connect(buttonBox, &QDialogButtonBox::accepted, [&]() {
        applyColumnFilter(column, getSelectedValues());
        filterDialog.accept();
    });

    connect(buttonBox->button(QDialogButtonBox::Apply), &QPushButton::clicked, [&]() {
        applyColumnFilter(column, getSelectedValues());
    });

    connect(clearFilterButton, &QPushButton::clicked, [&]() {
        clearColumnFilter(column);
        filterDialog.accept();
    });

    connect(buttonBox, &QDialogButtonBox::rejected, &filterDialog, &QDialog::reject);

    filterDialog.exec();
}

void MainWindow::applyColumnFilter(int column, const QStringList& selectedValues)
{
    if (selectedValues.isEmpty() || selectedValues.size() == m_columnUniqueValues[column].size()) {
        clearColumnFilter(column);
        return;
    }

    m_columnFilters[column] = selectedValues;

    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        bool showRow = true;
        for (auto it = m_columnFilters.begin(); it != m_columnFilters.end(); ++it) {
            int col = it.key();
            const QStringList& filterValues = it.value();
            QTableWidgetItem* item = dataTableWidget->item(row, col);
            if (item && !filterValues.contains(item->text())) {
                showRow = false;
                break;
            }
        }
        dataTableWidget->setRowHidden(row, !showRow);
        frozenTableWidget->setRowHidden(row, !showRow);
    }

    syncRowHeights();

    int visibleRows = 0;
    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        if (!dataTableWidget->isRowHidden(row)) visibleRows++;
    }

    statusBar()->showMessage(QString("Filter applied on column %1: %2 of %3 rows visible")
                                 .arg(dataTableWidget->horizontalHeaderItem(column)->text())
                                 .arg(visibleRows)
                                 .arg(dataTableWidget->rowCount()), 3000);

    updateFilterIndicator(column, true);
}

void MainWindow::clearColumnFilter(int column)
{
    if (m_columnFilters.remove(column) > 0) {
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            bool showRow = true;
            for (auto it = m_columnFilters.begin(); it != m_columnFilters.end(); ++it) {
                int col = it.key();
                const QStringList& filterValues = it.value();
                QTableWidgetItem* item = dataTableWidget->item(row, col);
                if (item && !filterValues.contains(item->text())) {
                    showRow = false;
                    break;
                }
            }
            dataTableWidget->setRowHidden(row, !showRow);
            frozenTableWidget->setRowHidden(row, !showRow);
        }
        updateFilterIndicator(column, false);
        int visibleRows = 0;
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            if (!dataTableWidget->isRowHidden(row)) visibleRows++;
        }
        statusBar()->showMessage(QString("Filter cleared on column %1: %2 rows visible")
                                     .arg(dataTableWidget->horizontalHeaderItem(column)->text())
                                     .arg(visibleRows), 2000);
    }
}

void MainWindow::clearAllFilters()
{
    m_columnFilters.clear();
    for (int row = 0; row < dataTableWidget->rowCount(); row++) {
        dataTableWidget->setRowHidden(row, false);
        frozenTableWidget->setRowHidden(row, false);
    }
    for (int col = 0; col < dataTableWidget->columnCount(); col++) {
        updateFilterIndicator(col, false);
    }
    statusBar()->showMessage("All filters cleared", 2000);
}

void MainWindow::updateFilterIndicator(int column, bool hasFilter)
{
    QTableWidgetItem* headerItem = dataTableWidget->horizontalHeaderItem(column);
    if (headerItem) {
        QString text = headerItem->text();
        if (text.startsWith("🔍 ")) text = text.mid(2);
        if (hasFilter) {
            headerItem->setText("🔍 " + text);
            headerItem->setForeground(QBrush(QColor(0, 120, 215)));
            headerItem->setToolTip("Filter active - Click to modify filter");
        } else {
            headerItem->setText(text);
            headerItem->setForeground(QBrush(QColor(0, 0, 0)));
            headerItem->setToolTip("");
        }
    }
}

void MainWindow::rebuildFilterMaps()
{
    m_columnUniqueValues.clear();
    for (int col = 0; col < dataTableWidget->columnCount(); col++) {
        QSet<QString> uniqueValues;
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            QTableWidgetItem* item = dataTableWidget->item(row, col);
            if (item && !item->text().isEmpty()) {
                uniqueValues.insert(item->text());
            }
        }
        m_columnUniqueValues[col] = uniqueValues;
    }
}

void MainWindow::onHeaderClicked(int column)
{
    QMenu contextMenu;

    QAction* filterAction = contextMenu.addAction("🔍 Filter Column...");
    QAction* clearFilterAction = contextMenu.addAction("❌ Clear Filter");
    QAction* clearAllFiltersAction = contextMenu.addAction("🗑️ Clear All Filters");
    contextMenu.addSeparator();
    QAction* sortAscAction = contextMenu.addAction("⬆️ Sort Ascending");
    QAction* sortDescAction = contextMenu.addAction("⬇️ Sort Descending");
    contextMenu.addSeparator();
    QAction* hideColumnAction = contextMenu.addAction("👁️ Hide Column");
    QAction* showAllColumnsAction = contextMenu.addAction("👁️ Show All Columns");

    QAction* selectedAction = contextMenu.exec(QCursor::pos());

    if (selectedAction == filterAction) {
        showColumnFilterMenu(column);
    } else if (selectedAction == clearFilterAction) {
        clearColumnFilter(column);
    } else if (selectedAction == clearAllFiltersAction) {
        clearAllFilters();
    } else if (selectedAction == sortAscAction || selectedAction == sortDescAction) {
        Qt::SortOrder order = (selectedAction == sortAscAction) ? Qt::AscendingOrder : Qt::DescendingOrder;
        QSet<int> visibleRowsBefore;
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            if (!dataTableWidget->isRowHidden(row)) {
                visibleRowsBefore.insert(row);
            }
        }
        dataTableWidget->sortByColumn(column, order);
        syncFrozenTableAfterSort();
        for (int row = 0; row < dataTableWidget->rowCount(); row++) {
            bool wasVisible = visibleRowsBefore.contains(row);
            dataTableWidget->setRowHidden(row, !wasVisible);
            frozenTableWidget->setRowHidden(row, !wasVisible);
        }
        statusBar()->showMessage(QString("Sorted by %1 (%2)").arg(dataTableWidget->horizontalHeaderItem(column)->text())
                                     .arg(order == Qt::AscendingOrder ? "Ascending" : "Descending"), 2000);
    } else if (selectedAction == hideColumnAction) {
        dataTableWidget->setColumnHidden(column, true);
        statusBar()->showMessage(QString("Column '%1' hidden").arg(dataTableWidget->horizontalHeaderItem(column)->text()), 2000);
    } else if (selectedAction == showAllColumnsAction) {
        for (int col = 0; col < dataTableWidget->columnCount(); col++) {
            dataTableWidget->setColumnHidden(col, false);
        }
        statusBar()->showMessage("All columns shown", 2000);
    }
}

void MainWindow::browseFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open File", QDir::homePath(),
                                                    "Data Files (*.txt *.csv *.TXT *.CSV);;All Files (*.*)");
    if(fileName.isEmpty()) return;
    currentFile = fileName;
    loadFile();
}

void MainWindow::exportCSV()
{
    if(frozenTableWidget->rowCount() == 0) {
        QMessageBox::warning(this, "Export Error", "No data to export");
        return;
    }

    QString fileName = QFileDialog::getSaveFileName(this, "Export CSV", "", "CSV Files (*.csv)");
    if(fileName.isEmpty()) return;
    if(!fileName.endsWith(".csv", Qt::CaseInsensitive)) fileName += ".csv";

    QFile file(fileName);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QMessageBox::warning(this, "Error", "Cannot save file");
        return;
    }

    QTextStream out(&file);
    out << headers.join(",") << "\n";

    for(int row = 0; row < frozenTableWidget->rowCount(); row++) {
        if (dataTableWidget->isRowHidden(row)) continue;
        QStringList rowData;
        rowData << (frozenTableWidget->item(row, 0) ? frozenTableWidget->item(row, 0)->text() : "");
        rowData << (frozenTableWidget->item(row, 1) ? frozenTableWidget->item(row, 1)->text() : "");
        for(int col = 0; col < dataTableWidget->columnCount(); col++) {
            QTableWidgetItem *item = dataTableWidget->item(row, col);
            rowData << (item ? item->text() : "");
        }
        out << rowData.join(",") << "\n";
    }
    file.close();
    QMessageBox::information(this, "Success", "CSV exported successfully!");
}

void MainWindow::updateColorButtons()
{
    if (rowHighlightColorButton) {
        rowHighlightColorButton->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #333; border-radius: 4px; }").arg(m_rowHighlightColor.name()));
    }
    if (columnHighlightColorButton) {
        columnHighlightColorButton->setStyleSheet(QString("QPushButton { background-color: %1; border: 2px solid #333; border-radius: 4px; }").arg(m_columnHighlightColor.name()));
    }
}