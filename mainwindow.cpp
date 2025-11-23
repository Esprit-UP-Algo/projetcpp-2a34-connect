#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "sponsor.h"
#include "employes.h"
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QTableWidgetItem>
#include <QDebug>
#include <QRegularExpression>
#include <QFileDialog>
#include <QStandardPaths>
#include <QDesktopServices>
#include <QUrl>
#include <QFormLayout>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QTimer>
#include <QStatusBar>
#include <QCheckBox>
#include <QHeaderView>
#include <QPainterPath>
#include <QLinearGradient>
#include <QRadialGradient>

// ==================== IMPLÉMENTATION DE CURVEWIDGET ====================

CurveWidget::CurveWidget(QWidget *parent)
    : QWidget(parent), m_color(Qt::blue)
{
    setMinimumSize(400, 300);
}

void CurveWidget::setData(const QVector<QPointF> &data, const QString &title, const QColor &color)
{
    m_data = data;
    m_title = title;
    m_color = color;
    update();
}

void CurveWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);

    // Fond avec dégradé
    QLinearGradient gradient(0, 0, width(), height());
    gradient.setColorAt(0, QColor(255, 255, 255, 200));
    gradient.setColorAt(1, QColor(240, 240, 255, 200));
    painter.fillRect(rect(), gradient);

    // Bordure
    painter.setPen(QPen(QColor(125, 79, 238), 2));
    painter.drawRoundedRect(rect().adjusted(1, 1, -1, -1), 10, 10);

    if (m_data.isEmpty()) {
        painter.setPen(Qt::gray);
        painter.drawText(rect(), Qt::AlignCenter, "Aucune donnée disponible");
        return;
    }

    // Calcul des limites
    double minX = m_data[0].x(), maxX = m_data[0].x();
    double minY = m_data[0].y(), maxY = m_data[0].y();

    for (const QPointF &point : m_data) {
        minX = qMin(minX, point.x());
        maxX = qMax(maxX, point.x());
        minY = qMin(minY, point.y());
        maxY = qMax(maxY, point.y());
    }

    // Ajouter une marge
    double xRange = maxX - minX;
    double yRange = maxY - minY;
    if (xRange == 0) xRange = 1;
    if (yRange == 0) yRange = 1;

    minY = qMax(0.0, minY - yRange * 0.1);
    maxY = maxY + yRange * 0.1;

    // Zone de dessin avec marges
    QRectF plotRect(50, 40, width() - 80, height() - 80);

    // Dessiner la grille
    painter.setPen(QPen(QColor(200, 200, 200), 1));
    for (int i = 0; i <= 5; ++i) {
        double y = plotRect.bottom() - (plotRect.height() * i / 5);
        painter.drawLine(plotRect.left(), y, plotRect.right(), y);
    }

    // Dessiner les axes
    painter.setPen(QPen(Qt::black, 2));
    painter.drawLine(plotRect.left(), plotRect.bottom(), plotRect.right(), plotRect.bottom()); // Axe X
    painter.drawLine(plotRect.left(), plotRect.top(), plotRect.left(), plotRect.bottom());     // Axe Y

    // Dessiner la courbe
    QPainterPath path;
    bool firstPoint = true;

    for (const QPointF &point : m_data) {
        double x = plotRect.left() + ((point.x() - minX) / xRange) * plotRect.width();
        double y = plotRect.bottom() - ((point.y() - minY) / (maxY - minY)) * plotRect.height();

        if (firstPoint) {
            path.moveTo(x, y);
            firstPoint = false;
        } else {
            path.lineTo(x, y);
        }
    }

    // Dessiner la ligne de la courbe
    QPen curvePen(m_color, 3);
    curvePen.setCapStyle(Qt::RoundCap);
    painter.setPen(curvePen);
    painter.drawPath(path);

    // Dessiner les points
    painter.setBrush(m_color);
    painter.setPen(QPen(Qt::white, 2));

    for (const QPointF &point : m_data) {
        double x = plotRect.left() + ((point.x() - minX) / xRange) * plotRect.width();
        double y = plotRect.bottom() - ((point.y() - minY) / (maxY - minY)) * plotRect.height();

        painter.drawEllipse(QPointF(x, y), 6, 6);

        // Afficher les valeurs sur les points
        painter.setPen(Qt::black);
        painter.drawText(QRectF(x - 20, y - 25, 40, 20),
                         Qt::AlignCenter,
                         QString::number(point.y(), 'f', 0));
        painter.setPen(QPen(Qt::white, 2));
    }

    // Titre
    painter.setPen(QColor(125, 79, 238));
    QFont titleFont = painter.font();
    titleFont.setBold(true);
    titleFont.setPointSize(10);
    painter.setFont(titleFont);
    painter.drawText(QRect(0, 10, width(), 20), Qt::AlignCenter, m_title);

    // Labels des axes
    painter.setPen(Qt::black);
    QFont labelFont = painter.font();
    labelFont.setPointSize(8);
    painter.setFont(labelFont);

    painter.drawText(QRectF(0, plotRect.bottom() + 5, width(), 20),
                     Qt::AlignCenter, "Période");
    painter.save();
    painter.translate(20, height() / 2);
    painter.rotate(-90);
    painter.drawText(QRectF(-100, 0, 200, 20), Qt::AlignCenter, "Valeurs");
    painter.restore();
}

// ==================== CONSTRUCTEUR ET DESTRUCTEUR ====================

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , idEmployeSelectionne(-1)
    , idSponsorSelectionne(-1)
    , chatbot(new Chatbot())
    , smsManager(new SMSManager())
{
    ui->setupUi(this);

    connecterSignaux();
    initEmployesTable();
    initSponsorsTable();
    afficherEmployes();
    afficherSponsors();

    ui->lineEdit->setText("Bonjour ! Comment puis-je vous aider ?");
    ui->lineEdit->setReadOnly(true);
    ui->lineEdit_2->clear();
    ui->lineEdit_2->setPlaceholderText("Tapez votre question ici...");

    // Mettre à jour le texte du bouton stats
    ui->pushButton_stats->setText("📊 Stats Graphiques");

    qDebug() << "✅ Application initialisée avec succès";
}

MainWindow::~MainWindow()
{
    delete chatbot;
    delete smsManager;
    delete ui;
}

// ==================== CONNEXION DES SIGNAUX ====================

void MainWindow::connecterSignaux()
{
    // Navigation
    connect(ui->sidebarButton, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_clicked);
    connect(ui->sidebarButton_2, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_2_clicked);
    connect(ui->sidebarButton_3, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_3_clicked);
    connect(ui->sidebarButton_4, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_4_clicked);
    connect(ui->sidebarButton_5, &QPushButton::clicked, this, &MainWindow::on_sidebarButton_5_clicked);
    connect(ui->sidebarButton12, &QPushButton::clicked, this, &MainWindow::on_sidebarButton12_clicked);

    // Employés
    connect(ui->pushButton_11, &QPushButton::clicked, this, &MainWindow::on_pushButton_11_clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &MainWindow::on_pushButton_8_clicked);
    connect(ui->pushButton_10, &QPushButton::clicked, this, &MainWindow::on_pushButton_10_clicked);
    connect(ui->pushButton_12, &QPushButton::clicked, this, &MainWindow::on_pushButton_12_clicked);
    connect(ui->lineEdit_6, &QLineEdit::textChanged, this, &MainWindow::on_lineEdit_6_textChanged);
    connect(ui->tableWidget, &QTableWidget::clicked, this, &MainWindow::on_tableWidget_clicked);

    // Sponsors - CRUD
    connect(ui->pushButton_23, &QPushButton::clicked, this, &MainWindow::on_pushButton_23_clicked);
    connect(ui->pushButton_38, &QPushButton::clicked, this, &MainWindow::on_pushButton_38_clicked);
    connect(ui->pushButton_40, &QPushButton::clicked, this, &MainWindow::on_pushButton_40_clicked);
    connect(ui->sponsorTable_9, &QTableWidget::clicked, this, &MainWindow::on_sponsorTable_9_clicked);
    connect(ui->refreshbtn, &QPushButton::clicked, this, &MainWindow::on_refreshbtn_clicked);
    connect(ui->pushButton_stats, &QPushButton::clicked, this, &MainWindow::on_pushButton_stats_clicked);

    // Sponsors - FONCTIONNALITÉS
    connect(ui->pushButton_35, &QPushButton::clicked, this, &MainWindow::on_pushButton_35_clicked);
    connect(ui->pushButton_37, &QPushButton::clicked, this, &MainWindow::on_pushButton_37_clicked);
    connect(ui->searchEdit_7, &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_7_textChanged);
    connect(ui->pushButton_39, &QPushButton::clicked, this, &MainWindow::on_pushButton_39_clicked);
    connect(ui->pushButton_41, &QPushButton::clicked, this, &MainWindow::on_pushButton_41_clicked);
    connect(ui->searchEdit_8, &QLineEdit::textChanged, this, &MainWindow::on_searchEdit_8_textChanged);

    // Chatbot & SMS
    connect(ui->pushButton_25, &QPushButton::clicked, this, &MainWindow::on_pushButton_25_clicked);
    connect(ui->pushButton_26, &QPushButton::clicked, this, &MainWindow::on_pushButton_26_clicked);
    connect(ui->btnConfigSMS, &QPushButton::clicked, this, &MainWindow::on_btnConfigSMS_clicked);
    connect(ui->lineEdit_2, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_25_clicked);
    connect(ui->lineEdit_4, &QLineEdit::returnPressed, this, &MainWindow::on_pushButton_26_clicked);

    // Recherche sponsor
    connect(ui->pushButton_24, &QPushButton::clicked, this, &MainWindow::on_pushButton_24_clicked);

    // Configuration Gemini
    connect(ui->ConfigurerGemini, &QPushButton::clicked, this, &MainWindow::on_ConfigurerGemini_clicked);
}

// ==================== CONTRÔLE DE SAISIE SPONSORS ====================

bool MainWindow::validerFormulaireSponsor()
{
    // Validation nom
    QString nom = ui->nom->text().trimmed();
    if (!Sponsor::validerNom(nom)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le nom doit contenir au moins 2 caractères et ne peut pas contenir de chiffres !");
        ui->nom->setFocus();
        return false;
    }

    // Validation prénom
    QString prenom = ui->prenom->text().trimmed();
    if (!Sponsor::validerPrenom(prenom)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le prénom doit contenir au moins 2 caractères et ne peut pas contenir de chiffres !");
        ui->prenom->setFocus();
        return false;
    }

    // Validation email
    QString email = ui->email->text().trimmed();
    if (!Sponsor::validerEmail(email)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ L'email n'est pas valide ! Format attendu : exemple@domaine.com");
        ui->email->setFocus();
        return false;
    }

    // Validation catégorie
    QString categorie = ui->categorie->text().trimmed();
    if (!Sponsor::validerCategorie(categorie)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La catégorie ne peut pas être vide !");
        ui->categorie->setFocus();
        return false;
    }

    // Validation budget
    bool budgetOk;
    int budget = ui->budget->text().toInt(&budgetOk);
    if (!budgetOk || !Sponsor::validerBudget(budget)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le budget doit être un nombre positif !");
        ui->budget->setFocus();
        return false;
    }

    // Validation dates
    QDate debut = ui->debut->date();
    QDate fin = ui->fin->date();
    QDate aujourdhui = QDate::currentDate();

    // Vérifier que les dates sont valides
    if (!debut.isValid() || !fin.isValid()) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Les dates ne sont pas valides !");
        ui->debut->setFocus();
        return false;
    }

    // Vérifier que la date de début n'est pas dans le passé
    if (debut < aujourdhui) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La date de début ne peut pas être dans le passé !");
        ui->debut->setFocus();
        return false;
    }

    // Vérifier que la date de fin est après la date de début
    if (fin <= debut) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La date de fin doit être après la date de début !");
        ui->fin->setFocus();
        return false;
    }

    // Vérifier que le contrat a une durée raisonnable (max 5 ans)
    if (debut.daysTo(fin) > 365 * 5) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La durée du contrat ne peut pas dépasser 5 ans !");
        ui->fin->setFocus();
        return false;
    }

    return true;
}

bool MainWindow::validerFormulaireModificationSponsor()
{
    // Validation ID
    QString idStr = ui->ids->text().trimmed();
    if (!Sponsor::validerID(idStr)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ L'ID doit être un nombre positif !");
        ui->ids->setFocus();
        return false;
    }

    // Validation nom
    QString nom = ui->nom2->text().trimmed();
    if (!Sponsor::validerNom(nom)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le nom doit contenir au moins 2 caractères et ne peut pas contenir de chiffres !");
        ui->nom2->setFocus();
        return false;
    }

    // Validation prénom
    QString prenom = ui->prenom2->text().trimmed();
    if (!Sponsor::validerPrenom(prenom)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le prénom doit contenir au moins 2 caractères et ne peut pas contenir de chiffres !");
        ui->prenom2->setFocus();
        return false;
    }

    // Validation email
    QString email = ui->email2->text().trimmed();
    if (!Sponsor::validerEmail(email)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ L'email n'est pas valide ! Format attendu : exemple@domaine.com");
        ui->email2->setFocus();
        return false;
    }

    // Validation catégorie
    QString categorie = ui->categorie2->text().trimmed();
    if (!Sponsor::validerCategorie(categorie)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La catégorie ne peut pas être vide !");
        ui->categorie2->setFocus();
        return false;
    }

    // Validation budget
    bool budgetOk;
    int budget = ui->budget2->text().toInt(&budgetOk);
    if (!budgetOk || !Sponsor::validerBudget(budget)) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Le budget doit être un nombre positif !");
        ui->budget2->setFocus();
        return false;
    }

    // Validation dates
    QDate debut = ui->debut2->date();
    QDate fin = ui->fin2->date();
    QDate aujourdhui = QDate::currentDate();

    // Vérifier que les dates sont valides
    if (!debut.isValid() || !fin.isValid()) {
        QMessageBox::warning(this, "Erreur de validation", "❌ Les dates ne sont pas valides !");
        ui->debut2->setFocus();
        return false;
    }

    // Pour la modification, on permet les dates passées (contrats existants)
    // Mais on vérifie quand même la cohérence

    // Vérifier que la date de fin est après la date de début
    if (fin <= debut) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La date de fin doit être après la date de début !");
        ui->fin2->setFocus();
        return false;
    }

    // Vérifier que le contrat a une durée raisonnable (max 5 ans)
    if (debut.daysTo(fin) > 365 * 5) {
        QMessageBox::warning(this, "Erreur de validation", "❌ La durée du contrat ne peut pas dépasser 5 ans !");
        ui->fin2->setFocus();
        return false;
    }

    return true;
}

// ==================== CONFIGURATION GEMINI ====================

void MainWindow::on_ConfigurerGemini_clicked()
{
    qDebug() << "=== CONFIGURATION API GEMINI ===";

    QDialog configDialog(this);
    configDialog.setWindowTitle("Configuration API Gemini");
    configDialog.setFixedSize(500, 400);

    QFormLayout *layout = new QFormLayout(&configDialog);

    QLineEdit *editApiKey = new QLineEdit();
    editApiKey->setPlaceholderText("Entrez votre clé API Google Gemini...");
    editApiKey->setEchoMode(QLineEdit::PasswordEchoOnEdit);

    if (chatbot->estConfigure()) {
        editApiKey->setPlaceholderText("Clé déjà configurée - Entrez une nouvelle clé si nécessaire");
    }

    QComboBox *comboModel = new QComboBox();
    comboModel->addItems({"gemini-2.0-flash", "gemini-1.5-pro", "gemini-pro"});
    comboModel->setCurrentText("gemini-2.0-flash");

    QLabel *labelInfo = new QLabel(
        "🔧 Configuration de l'API Google Gemini\n\n"
        "Pour obtenir une clé API :\n"
        "1. Allez sur https://aistudio.google.com/\n"
        "2. Créez un compte ou connectez-vous\n"
        "3. Cliquez sur 'Obtenir une clé API'\n"
        "4. Créez un nouveau projet si nécessaire\n"
        "5. Générez une clé API\n"
        "6. Collez-la dans le champ ci-dessus\n\n"
        "💡 Utilisez 'gemini-2.0-flash' pour de meilleures performances\n"
        "🔒 Votre clé est sauvegardée localement");
    labelInfo->setWordWrap(true);
    labelInfo->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 10px; border-radius: 5px; }");

    layout->addRow("🔑 Clé API Gemini:", editApiKey);
    layout->addRow("🤖 Modèle:", comboModel);
    layout->addRow(labelInfo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &configDialog);
    layout->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &configDialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &configDialog, &QDialog::reject);

    if (configDialog.exec() == QDialog::Accepted) {
        QString apiKey = editApiKey->text().trimmed();
        QString model = comboModel->currentText();

        if (apiKey.isEmpty() && !chatbot->estConfigure()) {
            QMessageBox::warning(this, "Attention", "⚠️ La clé API ne peut pas être vide !");
            return;
        }

        chatbot->configurerAPI(apiKey, model);

        if (!apiKey.isEmpty()) {
            QMessageBox::information(this, "Test en cours",
                                     "🔧 Test de connexion à l'API Gemini...\n\n"
                                     "Veuillez patienter quelques secondes.");

            QString testReponse = chatbot->obtenirReponse("Dis bonjour en français", -1);

            if (testReponse.contains("Erreur") || testReponse.contains("❌")) {
                QMessageBox::warning(this, "Test échoué",
                                     "❌ La connexion à l'API Gemini a échoué.\n\n"
                                     "Vérifiez :\n"
                                     "• Votre connexion Internet\n"
                                     "• La validité de votre clé API\n"
                                     "• Que l'API Gemini est activée\n\n"
                                     "Le chatbot utilisera le mode basique.");
            } else {
                QMessageBox::information(this, "Test réussi",
                                         "✅ Connexion à Gemini établie avec succès !\n\n"
                                         "Le chatbot utilisera maintenant l'IA avancée.\n\n"
                                         "Modèle: " + model);

                ui->lineEdit->setText("🤖 Bonjour ! Je suis maintenant connecté à Gemini AI. Comment puis-je vous aider ?");
            }
        } else {
            QMessageBox::information(this, "Configuration mise à jour",
                                     "✅ Configuration mise à jour !\n\n"
                                     "Modèle: " + model);
        }

        qDebug() << "🔧 API Gemini configurée - Modèle:" << model;
    } else {
        qDebug() << "Configuration Gemini annulée";
    }
}

// ==================== CHATBOT ====================

void MainWindow::on_pushButton_25_clicked()
{
    qDebug() << "=== CHATBOT - ENVOI MESSAGE ===";

    if (!chatbot) {
        QMessageBox::warning(this, "Erreur", "❌ Chatbot non initialisé !");
        return;
    }

    QString question = ui->lineEdit_2->text().trimmed();

    if (question.isEmpty()) {
        return;
    }

    qDebug() << "Question utilisateur:" << question;
    qDebug() << "Sponsor sélectionné:" << idSponsorSelectionne;

    QString reponse = chatbot->obtenirReponse(question, idSponsorSelectionne);

    qDebug() << "Réponse chatbot:" << reponse;

    QString conversation = ui->lineEdit->text();

    if (conversation == "Bonjour ! Comment puis-je vous aider ?" ||
        conversation == "🤖 Bonjour ! Je suis maintenant connecté à Gemini AI. Comment puis-je vous aider ?" ||
        conversation == "🤖 Bonjour ! Mode basique activé. Configurez Gemini pour l'IA avancée.") {
        conversation = "";
    }

    if (!conversation.isEmpty()) {
        conversation += "\n\n";
    }
    conversation += "👤 VOUS: " + question;
    conversation += "\n\n🤖 BOT: " + reponse;

    ui->lineEdit->setText(conversation);
    ui->lineEdit_2->clear();

    if (idSponsorSelectionne > 0) {
        chatbot->sauvegarderConversation(idSponsorSelectionne, question, reponse);
        qDebug() << "✅ Conversation sauvegardée pour sponsor" << idSponsorSelectionne;
    }
}

// ==================== SMS ====================

void MainWindow::on_pushButton_26_clicked()
{
    qDebug() << "=== SMS - ENVOI ===";

    if (!smsManager) {
        QMessageBox::warning(this, "Erreur", "❌ SMS Manager non initialisé !");
        return;
    }

    QString numero = ui->lineEdit_3->text().trimmed();
    QString message = ui->lineEdit_4->text().trimmed();

    if (numero.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone est obligatoire !");
        ui->lineEdit_3->setFocus();
        return;
    }

    if (!SMSManager::validerNumero(numero)) {
        QMessageBox::warning(this, "Erreur", "Le numéro de téléphone est invalide !");
        ui->lineEdit_3->setFocus();
        return;
    }

    if (message.isEmpty()) {
        QMessageBox::warning(this, "Erreur", "Le message est obligatoire !");
        ui->lineEdit_4->setFocus();
        return;
    }

    if (!SMSManager::validerMessage(message)) {
        QMessageBox::warning(this, "Erreur", "Le message est trop long (max 160 caractères) !");
        ui->lineEdit_4->setFocus();
        return;
    }

    qDebug() << "Numéro:" << numero;
    qDebug() << "Message:" << message;

    bool succes = smsManager->envoyerSMS(numero, message);

    if (succes) {
        QMessageBox::information(this, "Succès", "✅ SMS envoyé avec succès !");
        ui->lineEdit_3->clear();
        ui->lineEdit_4->clear();
        qDebug() << "✅ SMS envoyé avec succès";
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'envoi du SMS !");
        qDebug() << "❌ Échec envoi SMS";
    }
}

void MainWindow::on_btnConfigSMS_clicked()
{
    QDialog configDialog(this);
    configDialog.setWindowTitle("Configuration SMS API");
    configDialog.setFixedSize(400, 300);  // Fixed line

    QFormLayout *layout = new QFormLayout(&configDialog);

    QComboBox *comboProvider = new QComboBox();
    comboProvider->addItems({"Twilio", "Nexmo/Vonage", "Test"});

    QLineEdit *editApiKey = new QLineEdit();
    editApiKey->setPlaceholderText("Account SID (Twilio) ou API Key (Nexmo)");

    QLineEdit *editApiSecret = new QLineEdit();
    editApiSecret->setEchoMode(QLineEdit::Password);
    editApiSecret->setPlaceholderText("Auth Token (Twilio) ou API Secret (Nexmo)");

    QLineEdit *editFromNumber = new QLineEdit();
    editFromNumber->setPlaceholderText("+33612345678");

    QLabel *labelInfo = new QLabel("Pour Twilio: créez un compte sur twilio.com\nPour Nexmo: créez un compte sur vonage.com");
    labelInfo->setWordWrap(true);

    layout->addRow("Fournisseur:", comboProvider);
    layout->addRow("Clé API:", editApiKey);
    layout->addRow("Secret API:", editApiSecret);
    layout->addRow("Numéro expéditeur:", editFromNumber);
    layout->addRow(labelInfo);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, &configDialog);
    layout->addRow(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &configDialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &configDialog, &QDialog::reject);

    if (configDialog.exec() == QDialog::Accepted) {
        smsManager->configurerAPI(
            comboProvider->currentText(),
            editApiKey->text(),
            editApiSecret->text(),
            editFromNumber->text()
            );
        QMessageBox::information(this, "Succès", "Configuration SMS sauvegardée !");
    }
}

// ==================== NAVIGATION ====================

void MainWindow::on_sidebarButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
    qDebug() << "Navigation: Management Page";
}

void MainWindow::on_sidebarButton_2_clicked() {
    ui->stackedWidget->setCurrentIndex(1);
    qDebug() << "Navigation: Finance Page";
}

void MainWindow::on_sidebarButton_3_clicked() {
    ui->stackedWidget->setCurrentIndex(2);
    qDebug() << "Navigation: Content Creator Page";
}

void MainWindow::on_sidebarButton_4_clicked() {
    ui->stackedWidget->setCurrentIndex(3);
    afficherSponsors();
    qDebug() << "Navigation: Sponsor Page";
}

void MainWindow::on_sidebarButton_5_clicked() {
    ui->stackedWidget->setCurrentIndex(4);
    qDebug() << "Navigation: Deal Page";
}

void MainWindow::on_sidebarButton12_clicked() {
    ui->stackedWidget->setCurrentWidget(ui->employeePage);
    afficherEmployes();
    qDebug() << "Navigation: Employee Page";
}

// ==================== INITIALISATION TABLES ====================

void MainWindow::initEmployesTable()
{
    ui->tableWidget->setColumnCount(9);
    QStringList headers;
    headers << "ID" << "Prénom" << "Date" << "Email" << "Tél" << "Sexe" << "Poste" << "Salaire" << "Superviseur";
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    qDebug() << "✅ Table employés initialisée";
}

void MainWindow::initSponsorsTable()
{
    ui->sponsorTable_9->setColumnCount(8);
    QStringList headers;
    headers << "ID" << "Nom" << "Prénom" << "Catégorie" << "Email" << "Début" << "Fin" << "Budget";
    ui->sponsorTable_9->setHorizontalHeaderLabels(headers);
    ui->sponsorTable_9->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->sponsorTable_9->setSelectionMode(QAbstractItemView::SingleSelection);
    qDebug() << "✅ Table sponsors initialisée";
}

// ==================== AFFICHAGE DONNÉES ====================

void MainWindow::afficherEmployes()
{
    qDebug() << "=== AFFICHAGE EMPLOYÉS ===";

    QSqlQuery query;
    QString sql = "SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye";

    if (!query.exec(sql)) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "❌ Impossible de charger les employés:\n" + query.lastError().text());
        return;
    }

    ui->tableWidget->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->tableWidget->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Employés affichés:" << row << "lignes";
}

void MainWindow::afficherSponsors()
{
    qDebug() << "=== AFFICHAGE SPONSORS ===";

    QSqlQuery query;
    QString sql = "SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor";

    if (!query.exec(sql)) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "❌ Impossible de charger les sponsors:\n" + query.lastError().text());
        return;
    }

    ui->sponsorTable_9->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->sponsorTable_9->insertRow(row);
        for (int col = 0; col < 8; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->sponsorTable_9->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Sponsors affichés:" << row << "lignes";
}

// ==================== EMPLOYÉS ====================

void MainWindow::on_pushButton_11_clicked() // Ajouter
{
    QString prenom = ui->lineEdit_13->text().trimmed();
    QString email = ui->lineEdit_17->text().trimmed();
    QString telephone = ui->lineEdit_18->text().trimmed();
    QString sexe = ui->lineEdit_19->text().trimmed();
    QString poste = ui->lineEdit_14->text().trimmed();
    double salaire = ui->lineEdit_20->text().toDouble();

    Employes emp(prenom, email, telephone, sexe, poste, salaire, QDate::currentDate());

    if (emp.ajouter()) {
        afficherEmployes();
        clearEmployeFields();
        QMessageBox::information(this, "Succès", "✅ Employé ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'ajout");
    }
}

void MainWindow::on_pushButton_8_clicked() // Modifier
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé !");
        return;
    }

    QString email = ui->lineEdit_17->text().trimmed();
    QString telephone = ui->lineEdit_18->text().trimmed();
    QString poste = ui->lineEdit_14->text().trimmed();
    double salaire = ui->lineEdit_20->text().toDouble();

    Employes emp;
    if (emp.modifier(idEmployeSelectionne, email, telephone, poste, salaire, "")) {
        afficherEmployes();
        QMessageBox::information(this, "Succès", "✅ Employé modifié !");
    }
}

void MainWindow::on_pushButton_10_clicked() // Supprimer
{
    if (idEmployeSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un employé !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer cet employé ?") == QMessageBox::Yes) {
        Employes emp;
        if (emp.supprimer(idEmployeSelectionne)) {
            afficherEmployes();
            clearEmployeFields();
            idEmployeSelectionne = -1;
            QMessageBox::information(this, "Succès", "✅ Employé supprimé !");
        }
    }
}

void MainWindow::on_pushButton_12_clicked() // Trier
{
    QSqlQuery query;
    query.exec("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye ORDER BY SALAIRE DESC");

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
    QMessageBox::information(this, "Succès", "✅ Trié par salaire");
}

void MainWindow::on_lineEdit_6_textChanged(const QString &text)
{
    if (text.isEmpty()) {
        afficherEmployes();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT IDEMPLOYE, PRENOM, DATEDEMBAUCHE, EMAIL, TELEPHONE, SEXE, POSTE, SALAIRE, IDSUPERVISEUR FROM empolye WHERE UPPER(PRENOM) LIKE UPPER(:search)");
    query.bindValue(":search", "%" + text + "%");
    query.exec();

    ui->tableWidget->setRowCount(0);
    int row = 0;
    while (query.next()) {
        ui->tableWidget->insertRow(row);
        for (int col = 0; col < 9; col++) {
            ui->tableWidget->setItem(row, col, new QTableWidgetItem(query.value(col).toString()));
        }
        row++;
    }
}

void MainWindow::on_tableWidget_clicked(const QModelIndex &index)
{
    int row = index.row();
    idEmployeSelectionne = ui->tableWidget->item(row, 0)->text().toInt();

    ui->lineEdit_13->setText(ui->tableWidget->item(row, 1)->text());
    ui->lineEdit_17->setText(ui->tableWidget->item(row, 3)->text());
    ui->lineEdit_18->setText(ui->tableWidget->item(row, 4)->text());
    ui->lineEdit_19->setText(ui->tableWidget->item(row, 5)->text());
    ui->lineEdit_14->setText(ui->tableWidget->item(row, 6)->text());
    ui->lineEdit_20->setText(ui->tableWidget->item(row, 7)->text());
}

// ==================== SPONSORS - CRUD ====================

void MainWindow::on_pushButton_23_clicked() // Ajouter
{
    if (!validerFormulaireSponsor()) {
        return;
    }

    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prenom->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString categorie = ui->categorie->text().trimmed();
    QDate debut = ui->debut->date();
    QDate fin = ui->fin->date();
    int budget = ui->budget->text().toInt();

    Sponsor s(nom, prenom, categorie, email, debut, fin, budget);

    if (s.ajouter()) {
        afficherSponsors();
        clearSponsorFields();
        QMessageBox::information(this, "Succès", "✅ Sponsor ajouté avec succès !");
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'ajout");
    }
}

void MainWindow::on_pushButton_38_clicked() // Modifier
{
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor !");
        return;
    }

    if (!validerFormulaireModificationSponsor()) {
        return;
    }

    Sponsor s;
    if (s.modifier(idSponsorSelectionne, ui->nom2->text(), ui->prenom2->text(),
                   ui->categorie2->text(), ui->email2->text(), ui->debut2->date(),
                   ui->fin2->date(), ui->budget2->text().toInt())) {
        afficherSponsors();
        QMessageBox::information(this, "Succès", "✅ Sponsor modifié !");
    }
}

void MainWindow::on_pushButton_40_clicked() // Supprimer
{
    if (idSponsorSelectionne == -1) {
        QMessageBox::warning(this, "Erreur", "❌ Sélectionnez un sponsor !");
        return;
    }

    if (QMessageBox::question(this, "Confirmation", "Supprimer ce sponsor ?") == QMessageBox::Yes) {
        Sponsor s;
        if (s.supprimer(idSponsorSelectionne)) {
            afficherSponsors();
            clearSponsorFields();
            idSponsorSelectionne = -1;
            QMessageBox::information(this, "Succès", "✅ Sponsor supprimé !");
        }
    }
}

void MainWindow::on_sponsorTable_9_clicked(const QModelIndex &index)
{
    int row = index.row();
    idSponsorSelectionne = ui->sponsorTable_9->item(row, 0)->text().toInt();

    ui->ids->setText(ui->sponsorTable_9->item(row, 0)->text());
    ui->nom2->setText(ui->sponsorTable_9->item(row, 1)->text());
    ui->prenom2->setText(ui->sponsorTable_9->item(row, 2)->text());
    ui->categorie2->setText(ui->sponsorTable_9->item(row, 3)->text());
    ui->email2->setText(ui->sponsorTable_9->item(row, 4)->text());
    ui->debut2->setDate(QDate::fromString(ui->sponsorTable_9->item(row, 5)->text(), "yyyy-MM-dd"));
    ui->fin2->setDate(QDate::fromString(ui->sponsorTable_9->item(row, 6)->text(), "yyyy-MM-dd"));
    ui->budget2->setText(ui->sponsorTable_9->item(row, 7)->text());
}

void MainWindow::on_refreshbtn_clicked()
{
    afficherSponsors();
    QMessageBox::information(this, "Succès", "✅ Liste rafraîchie !");
}

// ==================== STATISTIQUES AVEC COURBES ====================

void MainWindow::on_pushButton_stats_clicked()
{
    qDebug() << "=== AFFICHAGE STATISTIQUES AVEC COURBES ===";

    afficherStatistiquesCourbes();
}

void MainWindow::afficherStatistiquesCourbes()
{
    // Créer une boîte de dialogue pour afficher les graphiques
    QDialog *statsDialog = new QDialog(this);
    statsDialog->setWindowTitle("📊 Statistiques des Sponsors - Vue Graphique");
    statsDialog->setMinimumSize(1200, 800);
    statsDialog->setStyleSheet(R"(
        QDialog {
            background: qlineargradient(x1:0, y1:0, x2:1, y2:1,
                stop:0 #667eea, stop:1 #764ba2);
        }
        QLabel {
            color: white;
            font: bold 14pt "Arial";
            background: transparent;
        }
        QGroupBox {
            background: rgba(255, 255, 255, 0.95);
            border: 2px solid #7D4FEE;
            border-radius: 15px;
            margin-top: 10px;
            font: bold 12pt "Arial";
            color: #7D4FEE;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top center;
            padding: 5px 10px;
            background-color: #7D4FEE;
            color: white;
            border-radius: 10px;
        }
        QPushButton {
            background-color: #7D4FEE;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 10px 20px;
            font: bold 12pt "Arial";
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #5A2BE2;
            transform: scale(1.05);
        }
    )");

    QVBoxLayout *mainLayout = new QVBoxLayout(statsDialog);

    // Titre
    QLabel *titleLabel = new QLabel("📈 Statistiques Détaillées des Sponsors");
    titleLabel->setAlignment(Qt::AlignCenter);
    titleLabel->setStyleSheet("font: bold 24pt 'Arial'; color: white; margin: 20px; background: transparent;");
    mainLayout->addWidget(titleLabel);

    // Layout pour les graphiques
    QGridLayout *gridLayout = new QGridLayout();
    gridLayout->setSpacing(15);

    // Graphique 1: Évolution des contrats
    QGroupBox *evolutionBox = new QGroupBox("📅 Évolution des Contrats par Mois");
    QVBoxLayout *evolutionLayout = new QVBoxLayout(evolutionBox);
    creerCourbeEvolutionContrats(evolutionLayout);
    gridLayout->addWidget(evolutionBox, 0, 0);

    // Graphique 2: Budgets
    QGroupBox *budgetBox = new QGroupBox("💰 Budgets par Catégorie");
    QVBoxLayout *budgetLayout = new QVBoxLayout(budgetBox);
    creerCourbeBudgets(budgetLayout);
    gridLayout->addWidget(budgetBox, 0, 1);

    // Graphique 3: Répartition par catégorie
    QGroupBox *categorieBox = new QGroupBox("📂 Répartition par Catégorie");
    QVBoxLayout *categorieLayout = new QVBoxLayout(categorieBox);
    creerGraphiqueCategories(categorieLayout);
    gridLayout->addWidget(categorieBox, 1, 0);

    // Graphique 4: Durée des contrats
    QGroupBox *dureeBox = new QGroupBox("⏱️ Durée des Contrats");
    QVBoxLayout *dureeLayout = new QVBoxLayout(dureeBox);
    creerGraphiqueDureeContrats(dureeLayout);
    gridLayout->addWidget(dureeBox, 1, 1);

    mainLayout->addLayout(gridLayout);

    // Statistiques résumées
    QGroupBox *resumeBox = new QGroupBox("📊 Résumé des Statistiques");
    resumeBox->setStyleSheet("QGroupBox { background: rgba(255, 255, 255, 0.9); }");
    QHBoxLayout *resumeLayout = new QHBoxLayout(resumeBox);

    QSqlQuery resumeQuery;
    resumeQuery.exec("SELECT COUNT(*), SUM(BUDGET), AVG(BUDGET) FROM sponsor");

    QString resumeText;
    if (resumeQuery.next()) {
        int totalSponsors = resumeQuery.value(0).toInt();
        double totalBudget = resumeQuery.value(1).toDouble();
        double avgBudget = resumeQuery.value(2).toDouble();

        resumeText = QString(
                         "🏢 <b>Total Sponsors:</b> %1\n\n"
                         "💵 <b>Budget Total:</b> %2 €\n\n"
                         "📈 <b>Budget Moyen:</b> %3 €\n\n"
                         "🎯 <b>Analyse:</b> Performance %4"
                         )
                         .arg(totalSponsors)
                         .arg(QString::number(totalBudget, 'f', 0))
                         .arg(QString::number(avgBudget, 'f', 0))
                         .arg(totalSponsors > 10 ? "📈 Excellente" : totalSponsors > 5 ? "✅ Bonne" : "⚠️ À améliorer");
    }

    QLabel *resumeLabel = new QLabel(resumeText);
    resumeLabel->setStyleSheet("font: 12pt 'Arial'; color: #333; padding: 15px; line-height: 1.8;");
    resumeLabel->setAlignment(Qt::AlignCenter);
    resumeLayout->addWidget(resumeLabel);

    mainLayout->addWidget(resumeBox);

    // Boutons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    QPushButton *closeButton = new QPushButton("Fermer");
    QPushButton *exportButton = new QPushButton("Exporter en PDF");

    closeButton->setStyleSheet(R"(
        QPushButton {
            background-color: #FF6B6B;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 25px;
            font: bold 12pt "Arial";
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #FF5252;
        }
    )");

    exportButton->setStyleSheet(R"(
        QPushButton {
            background-color: #4ECDC4;
            color: white;
            border: none;
            border-radius: 10px;
            padding: 12px 25px;
            font: bold 12pt "Arial";
            min-width: 120px;
        }
        QPushButton:hover {
            background-color: #26A69A;
        }
    )");

    buttonLayout->addWidget(exportButton);
    buttonLayout->addWidget(closeButton);
    buttonLayout->setAlignment(Qt::AlignCenter);
    buttonLayout->setSpacing(20);
    mainLayout->addLayout(buttonLayout);

    // Connexions
    connect(closeButton, &QPushButton::clicked, statsDialog, &QDialog::accept);
    connect(exportButton, &QPushButton::clicked, this, [this]() {
        exporterStatistiquesPDF();
    });

    statsDialog->exec();
    delete statsDialog;
}

void MainWindow::creerCourbeEvolutionContrats(QVBoxLayout *layout)
{
    // Récupérer les données d'évolution mensuelle
    QSqlQuery query;
    query.exec(R"(
        SELECT strftime('%Y-%m', DEBUTCONTRAT) as mois, COUNT(*)
        FROM sponsor
        WHERE DEBUTCONTRAT IS NOT NULL
        GROUP BY strftime('%Y-%m', DEBUTCONTRAT)
        ORDER BY mois
    )");

    QVector<QPointF> data;
    int pointIndex = 0;

    while (query.next()) {
        int count = query.value(1).toInt();
        data.append(QPointF(pointIndex, count));
        pointIndex++;
    }

    // Créer le widget de courbe
    CurveWidget *curveWidget = new CurveWidget();
    curveWidget->setData(data, "Évolution des Nouveaux Contrats", QColor(255, 107, 107));
    layout->addWidget(curveWidget);

    // Légende
    if (!data.isEmpty()) {
        QLabel *legendLabel = new QLabel("📈 Courbe montrant l'évolution du nombre de contrats par mois");
        legendLabel->setStyleSheet("font: 9pt 'Arial'; color: #666; padding: 5px;");
        legendLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(legendLabel);
    } else {
        QLabel *noDataLabel = new QLabel("Aucune donnée d'évolution disponible");
        noDataLabel->setStyleSheet("font: 12pt 'Arial'; color: #666; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noDataLabel);
    }
}

void MainWindow::creerCourbeBudgets(QVBoxLayout *layout)
{
    // Récupérer les données de budget par catégorie
    QSqlQuery query;
    query.exec("SELECT CATEGORIE, SUM(BUDGET) FROM sponsor GROUP BY CATEGORIE ORDER BY SUM(BUDGET) DESC");

    QVector<QPointF> data;
    int pointIndex = 0;
    QStringList categories;

    while (query.next()) {
        QString categorie = query.value(0).toString();
        double budget = query.value(1).toDouble();
        data.append(QPointF(pointIndex, budget));
        categories << categorie;
        pointIndex++;
    }

    // Créer le widget de courbe
    CurveWidget *curveWidget = new CurveWidget();
    curveWidget->setData(data, "Budgets par Catégorie", QColor(125, 79, 238));
    layout->addWidget(curveWidget);

    // Légende avec les catégories
    if (!data.isEmpty()) {
        QString legendText = "🏷️ Catégories: ";
        for (int i = 0; i < qMin(categories.size(), 3); ++i) {
            legendText += categories[i] + (i < qMin(categories.size(), 3) - 1 ? ", " : "");
        }
        if (categories.size() > 3) {
            legendText += " ...";
        }

        QLabel *legendLabel = new QLabel(legendText);
        legendLabel->setStyleSheet("font: 9pt 'Arial'; color: #666; padding: 5px;");
        legendLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(legendLabel);
    } else {
        QLabel *noDataLabel = new QLabel("Aucune donnée de budget disponible");
        noDataLabel->setStyleSheet("font: 12pt 'Arial'; color: #666; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noDataLabel);
    }
}

void MainWindow::creerGraphiqueCategories(QVBoxLayout *layout)
{
    // Récupérer les données de répartition
    QSqlQuery query;
    query.exec("SELECT CATEGORIE, COUNT(*) FROM sponsor GROUP BY CATEGORIE ORDER BY COUNT(*) DESC");

    QVector<QPointF> data;
    int pointIndex = 0;
    QStringList categories;

    while (query.next()) {
        QString categorie = query.value(0).toString();
        int count = query.value(1).toInt();
        data.append(QPointF(pointIndex, count));
        categories << categorie;
        pointIndex++;
    }

    // Créer le widget de courbe
    CurveWidget *curveWidget = new CurveWidget();
    curveWidget->setData(data, "Répartition par Catégorie", QColor(78, 205, 196));
    layout->addWidget(curveWidget);

    // Légende
    if (!data.isEmpty()) {
        int total = 0;
        for (const QPointF &point : data) {
            total += point.y();
        }

        QString legendText = QString("📊 Total: %1 sponsors - %2 catégories")
                                 .arg(total)
                                 .arg(categories.size());

        QLabel *legendLabel = new QLabel(legendText);
        legendLabel->setStyleSheet("font: 9pt 'Arial'; color: #666; padding: 5px;");
        legendLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(legendLabel);
    } else {
        QLabel *noDataLabel = new QLabel("Aucune donnée de catégorie disponible");
        noDataLabel->setStyleSheet("font: 12pt 'Arial'; color: #666; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noDataLabel);
    }
}

void MainWindow::creerGraphiqueDureeContrats(QVBoxLayout *layout)
{
    // Récupérer les données de durée
    QSqlQuery query;
    query.exec(R"(
        SELECT CATEGORIE,
               AVG(julianday(FINCONTRAT) - julianday(DEBUTCONTRAT)) as duree_moyenne
        FROM sponsor
        WHERE DEBUTCONTRAT IS NOT NULL AND FINCONTRAT IS NOT NULL
        GROUP BY CATEGORIE
        ORDER BY duree_moyenne DESC
    )");

    QVector<QPointF> data;
    int pointIndex = 0;

    while (query.next()) {
        double duree = query.value(1).toDouble();
        data.append(QPointF(pointIndex, duree));
        pointIndex++;
    }

    // Créer le widget de courbe
    CurveWidget *curveWidget = new CurveWidget();
    curveWidget->setData(data, "Durée Moyenne des Contrats (jours)", QColor(255, 167, 38));
    layout->addWidget(curveWidget);

    // Légende
    if (!data.isEmpty()) {
        double dureeMoyenneGlobale = 0;
        for (const QPointF &point : data) {
            dureeMoyenneGlobale += point.y();
        }
        dureeMoyenneGlobale /= data.size();

        QString legendText = QString("⏱️ Durée moyenne globale: %1 jours")
                                 .arg(QString::number(dureeMoyenneGlobale, 'f', 0));

        QLabel *legendLabel = new QLabel(legendText);
        legendLabel->setStyleSheet("font: 9pt 'Arial'; color: #666; padding: 5px;");
        legendLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(legendLabel);
    } else {
        QLabel *noDataLabel = new QLabel("Aucune donnée de durée disponible");
        noDataLabel->setStyleSheet("font: 12pt 'Arial'; color: #666; padding: 20px;");
        noDataLabel->setAlignment(Qt::AlignCenter);
        layout->addWidget(noDataLabel);
    }
}

void MainWindow::exporterStatistiquesPDF()
{
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Exporter les Statistiques",
                                                    QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation) + "/statistiques_sponsors.html",
                                                    "Fichiers HTML (*.html)");

    if (!fileName.isEmpty()) {
        Sponsor::exporterPDF(fileName);
        QMessageBox::information(this, "Succès",
                                 "✅ Statistiques exportées avec succès !\n\nFichier: " + fileName);
    }
}

// ==================== SPONSORS - EXPORT PDF PAR CONTRAT ====================

void MainWindow::exporterSponsorsPDFParContrat()
{
    qDebug() << "=== EXPORT PDF SPONSORS PAR CONTRAT ===";

    // Dialogue pour choisir le type de contrat
    QDialog dialog(this);
    dialog.setWindowTitle("Exporter les sponsors par type de contrat");
    dialog.setFixedSize(400, 300);

    QVBoxLayout *layout = new QVBoxLayout(&dialog);

    QLabel *label = new QLabel("Choisissez le type de contrat à exporter:");
    layout->addWidget(label);

    QComboBox *comboContrat = new QComboBox();
    comboContrat->addItem("Tous les sponsors", "tous");
    comboContrat->addItem("Contrats actifs", "actifs");
    comboContrat->addItem("Contrats expirés", "expires");
    comboContrat->addItem("Contrats bientôt expirés (30 jours)", "bientot_expires");
    comboContrat->addItem("Contrats futurs", "futurs");
    layout->addWidget(comboContrat);

    // Informations sur les filtres
    QLabel *infoLabel = new QLabel(
        "📋 Filtres disponibles:\n"
        "• Actifs: Contrats en cours\n"
        "• Expirés: Contrats terminés\n"
        "• Bientôt expirés: Dans les 30 jours\n"
        "• Futurs: Pas encore commencés"
        );
    infoLabel->setWordWrap(true);
    infoLabel->setStyleSheet("QLabel { background-color: #f0f0f0; padding: 10px; border-radius: 5px; }");
    layout->addWidget(infoLabel);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    layout->addWidget(buttons);

    connect(buttons, &QDialogButtonBox::accepted, &dialog, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, &dialog, &QDialog::reject);

    if (dialog.exec() != QDialog::Accepted) {
        return;
    }

    QString filtreContrat = comboContrat->currentData().toString();
    QString nomFiltre = comboContrat->currentText();

    // Choisir le fichier de destination
    QString documentsPath = QStandardPaths::writableLocation(QStandardPaths::DocumentsLocation);
    QString defaultFileName = documentsPath + "/Sponsors_" + nomFiltre.replace(" ", "_") + "_" +
                              QDate::currentDate().toString("yyyy-MM-dd") + ".html";

    QString nomFichier = QFileDialog::getSaveFileName(
        this,
        "Enregistrer la liste des sponsors - " + nomFiltre,
        defaultFileName,
        "Fichiers HTML (*.html);;Tous les fichiers (*)"
        );

    if (nomFichier.isEmpty()) {
        return;
    }

    if (!nomFichier.endsWith(".html", Qt::CaseInsensitive)) {
        nomFichier += ".html";
    }

    qDebug() << "Export avec filtre:" << filtreContrat << "- Fichier:" << nomFichier;

    if (Sponsor::exporterPDFParContrat(nomFichier, filtreContrat)) {
        QMessageBox::information(this, "Succès",
                                 QString("✅ Liste des sponsors exportée avec succès !\n\n"
                                         "Filtre: %1\n"
                                         "Fichier: %2\n\n"
                                         "Le fichier HTML peut être imprimé en PDF depuis votre navigateur.")
                                     .arg(nomFiltre)
                                     .arg(nomFichier));

        // Demander si on veut ouvrir le fichier
        QMessageBox::StandardButton ouvrir = QMessageBox::question(
            this,
            "Ouvrir le fichier",
            "Voulez-vous ouvrir le fichier HTML ?",
            QMessageBox::Yes | QMessageBox::No
            );

        if (ouvrir == QMessageBox::Yes) {
            QDesktopServices::openUrl(QUrl::fromLocalFile(nomFichier));
        }
    } else {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de l'export !");
    }
}

// ==================== SPONSORS - TRI ====================

void MainWindow::trierSponsorsParDate()
{
    qDebug() << "=== TRI SPONSORS PAR DATE ===";

    QSqlQuery query;
    QString sql = "SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET FROM sponsor ORDER BY DEBUTCONTRAT DESC";

    if (!query.exec(sql)) {
        qDebug() << "❌ ERREUR SQL:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "❌ Impossible de trier:\n" + query.lastError().text());
        return;
    }

    ui->sponsorTable_9->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->sponsorTable_9->insertRow(row);
        for (int col = 0; col < 8; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->sponsorTable_9->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Sponsors triés par date:" << row << "lignes";
    QMessageBox::information(this, "Succès", "✅ Sponsors triés par date (plus récents en premier)");
}

void MainWindow::on_pushButton_37_clicked() // TAB ADD - TRI PAR DATE
{
    trierSponsorsParDate();
}

void MainWindow::on_pushButton_41_clicked() // TAB MODIFY - TRI PAR DATE
{
    trierSponsorsParDate();
}

// ==================== SPONSORS - EXPORT PDF ====================

void MainWindow::on_pushButton_35_clicked() // TAB ADD - EXPORT PDF
{
    exporterSponsorsPDFParContrat(); // APPEL DIRECT
}

void MainWindow::on_pushButton_39_clicked() // TAB MODIFY - EXPORT PDF
{
    exporterSponsorsPDFParContrat(); // APPEL DIRECT
}

// ==================== FONCTIONS MANQUANTES ====================

void MainWindow::on_searchEdit_7_textChanged(const QString &text)
{
    QTimer::singleShot(300, this, [this, text]() {
        rechercherSponsors(text);
    });
}

void MainWindow::on_searchEdit_8_textChanged(const QString &text)
{
    QTimer::singleShot(300, this, [this, text]() {
        rechercherSponsors(text);
    });
}

void MainWindow::on_pushButton_24_clicked()
{
    qDebug() << "=== RECHERCHE SPONSOR (BOUTON pushButton_24) ===";

    QString recherche = ui->searchEdit_7->text().trimmed();

    if (recherche.isEmpty()) {
        QMessageBox::information(this, "Recherche", "⚠️ Veuillez entrer un nom à rechercher !");
        ui->searchEdit_7->setFocus();
        return;
    }

    qDebug() << "Recherche sponsor:" << recherche;

    Sponsor sponsor;
    QSqlQueryModel *model = sponsor.rechercherParNom(recherche);

    if (!model) {
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de la recherche !");
        return;
    }

    ui->sponsorTable_9->setRowCount(0);

    if (model->rowCount() == 0) {
        QMessageBox::information(this, "Recherche",
                                 QString("🔍 Aucun sponsor trouvé pour : \"%1\"").arg(recherche));
        delete model;
        return;
    }

    for (int row = 0; row < model->rowCount(); ++row) {
        ui->sponsorTable_9->insertRow(row);
        for (int col = 0; col < 8; ++col) {
            QTableWidgetItem *item = new QTableWidgetItem(model->data(model->index(row, col)).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->sponsorTable_9->setItem(row, col, item);
        }
    }

    QMessageBox::information(this, "Recherche",
                             QString("✅ %1 sponsor(s) trouvé(s) pour : %2")
                                 .arg(model->rowCount())
                                 .arg(recherche));

    delete model;
    qDebug() << "✅ Recherche terminée:" << model->rowCount() << "résultats";
}

// ==================== MÉTHODES AUXILIAIRES SPONSORS ====================

void MainWindow::rechercherSponsors(const QString &recherche)
{
    qDebug() << "=== RECHERCHE SPONSOR: " << recherche << " ===";

    if (recherche.trimmed().isEmpty()) {
        afficherSponsors();
        return;
    }

    QSqlQuery query;
    query.prepare("SELECT IDSPONSOR, NOM, PRENOM, CATEGORIE, EMAIL, DEBUTCONTRAT, FINCONTRAT, BUDGET "
                  "FROM sponsor "
                  "WHERE UPPER(NOM) LIKE UPPER(:search) OR UPPER(PRENOM) LIKE UPPER(:search) OR UPPER(CATEGORIE) LIKE UPPER(:search)");
    query.bindValue(":search", "%" + recherche.trimmed() + "%");

    if (!query.exec()) {
        qDebug() << "❌ Erreur recherche:" << query.lastError().text();
        QMessageBox::critical(this, "Erreur", "❌ Erreur lors de la recherche :\n" + query.lastError().text());
        return;
    }

    ui->sponsorTable_9->setRowCount(0);
    int row = 0;

    while (query.next()) {
        ui->sponsorTable_9->insertRow(row);
        for (int col = 0; col < 8; col++) {
            QTableWidgetItem *item = new QTableWidgetItem(query.value(col).toString());
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            ui->sponsorTable_9->setItem(row, col, item);
        }
        row++;
    }

    qDebug() << "✅ Résultats trouvés:" << row;

    if (row == 0) {
        statusBar()->showMessage(QString("Aucun sponsor trouvé pour \"%1\"").arg(recherche), 3000);
    } else {
        statusBar()->showMessage(QString("%1 sponsor(s) trouvé(s)").arg(row), 3000);
    }
}

// ==================== MÉTHODES AUXILIAIRES ====================

void MainWindow::clearEmployeFields()
{
    ui->lineEdit_13->clear();
    ui->lineEdit_14->clear();
    ui->lineEdit_17->clear();
    ui->lineEdit_18->clear();
    ui->lineEdit_19->clear();
    ui->lineEdit_20->clear();
}

void MainWindow::clearSponsorFields()
{
    ui->nom->clear();
    ui->prenom->clear();
    ui->email->clear();
    ui->categorie->clear();
    ui->budget2->clear();
}
