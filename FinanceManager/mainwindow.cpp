#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "connection.h"  // <- AJOUTEZ CETTE LIGNE
#include <QFileDialog>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QListWidgetItem>
#include <QtCharts/QPieSeries>
#include <QtCharts/QLineSeries>
#include <QtCharts/QValueAxis>
#include <QtCharts/QBarCategoryAxis>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("Finance Manager v1.0");

    notificationManager = new NotificationManager(this);
    predictionEngine = new PredictionEngine(this);

    initializeCharts();

    // =================== Menu Buttons ===================
    connect(ui->btnManagement, &QPushButton::clicked, this, &MainWindow::showManagementPage);
    connect(ui->btnFinance, &QPushButton::clicked, this, &MainWindow::showFinancePage);
    connect(ui->btnContentCreator, &QPushButton::clicked, this, &MainWindow::showContentCreatorPage);
    connect(ui->btnSponsor, &QPushButton::clicked, this, &MainWindow::showSponsorPage);
    connect(ui->btnEmployee, &QPushButton::clicked, this, &MainWindow::showEmployeePage);
    connect(ui->btnNotifications, &QPushButton::clicked, this, &MainWindow::showNotificationsPage);
    connect(ui->btnAnalytics, &QPushButton::clicked, this, &MainWindow::showAnalyticsPage);

    // =================== Finance Buttons ===================
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addInvoice);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editInvoice);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteInvoice);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(ui->btnSortByMontant, &QPushButton::clicked, this, &MainWindow::sortByAmount);
    connect(ui->btnSearchById, &QPushButton::clicked, this, &MainWindow::searchById);
    connect(ui->btnExportCSV, &QPushButton::clicked, this, &MainWindow::exportToCSV);
    connect(ui->btnDarkTheme, &QPushButton::clicked, this, &MainWindow::toggleDarkTheme);

    // =================== Notifications Buttons ===================
    connect(ui->btnMarkAllRead, &QPushButton::clicked, this, &MainWindow::markAllAsRead);
    connect(ui->listNotifications, &QListWidget::itemClicked, this, &MainWindow::onNotificationClicked);

    // Connect table selection to auto-fill form
    connect(ui->tableFactures, &QTableWidget::itemSelectionChanged, this, &MainWindow::onInvoiceSelected);

    // Initialize and load data
    initializeVisualStatistics();
    loadInvoicesFromDatabase();
    applyLightTheme();
    updateNotificationBadge();
}

void MainWindow::initializeCharts()
{
    statusChart = new QChart();
    statusChart->setTitle("📊 Invoices Distribution by Status");
    statusChart->setAnimationOptions(QChart::AllAnimations);

    statusChartView = new QChartView(statusChart);
    statusChartView->setRenderHint(QPainter::Antialiasing);
    statusChartView->setMinimumSize(400, 300);

    trendChart = new QChart();
    trendChart->setTitle("📈 Monthly Amounts Evolution");
    trendChart->setAnimationOptions(QChart::AllAnimations);

    trendChartView = new QChartView(trendChart);
    trendChartView->setRenderHint(QPainter::Antialiasing);
    trendChartView->setMinimumSize(400, 300);
}

// =================== MENU FUNCTIONS ===================
void MainWindow::showManagementPage() {
    ui->stackedWidget->setCurrentWidget(ui->managementPage);
    this->setWindowTitle("Finance Manager - Management");
}

void MainWindow::showFinancePage() {
    ui->stackedWidget->setCurrentWidget(ui->financePage);
    this->setWindowTitle("Finance Manager - Finance");
    loadInvoicesFromDatabase();
}

void MainWindow::showContentCreatorPage() {
    ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage);
    this->setWindowTitle("Finance Manager - Content Creators");
}

void MainWindow::showSponsorPage() {
    ui->stackedWidget->setCurrentWidget(ui->sponsorPage);
    this->setWindowTitle("Finance Manager - Sponsors");
}

void MainWindow::showEmployeePage() {
    ui->stackedWidget->setCurrentWidget(ui->employeePage);
    this->setWindowTitle("Finance Manager - Employees");
}

void MainWindow::showNotificationsPage() {
    ui->stackedWidget->setCurrentWidget(ui->notificationsPage);
    this->setWindowTitle("Finance Manager - Notifications");
    loadNotifications();
}

void MainWindow::showAnalyticsPage() {
    ui->stackedWidget->setCurrentWidget(ui->analyticsPage);
    this->setWindowTitle("Finance Manager - Analytics");
    updateAnalytics();
}

// =================== FINANCE FUNCTIONS ===================
void MainWindow::addInvoice() {
    QString id = ui->lineId->text().trimmed();
    QString amountText = ui->lineMontant->text().trimmed();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    // Validation
    if (id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Invoice ID is required!");
        return;
    }

    if (amountText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Amount is required!");
        return;
    }

    bool ok;
    double amount = amountText.toDouble(&ok);
    if (!ok || amount <= 0) {
        QMessageBox::warning(this, "Error", "Please enter a valid amount!");
        return;
    }

    if (dueDate <= issueDate) {
        QMessageBox::warning(this, "Error", "Due date must be after issue date!");
        return;
    }

    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();

    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not connected!");
        return;
    }

    // Check if invoice ID already exists
    QSqlQuery checkQuery(db);
    checkQuery.prepare("SELECT COUNT(*) FROM FACTURE WHERE id_facture = ?");
    checkQuery.addBindValue(id);

    if (checkQuery.exec() && checkQuery.next()) {
        if (checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Error", "Invoice ID already exists!");
            return;
        }
    }

    // Insert new invoice
    QSqlQuery query(db);
    query.prepare("INSERT INTO FACTURE (id_facture, montant, date_emission, date_echeance, statut, id_employe) "
                  "VALUES (?, ?, ?, ?, ?, ?)");
    query.addBindValue(id);
    query.addBindValue(amount);
    query.addBindValue(issueDate.toString("yyyy-MM-dd"));
    query.addBindValue(dueDate.toString("yyyy-MM-dd"));
    query.addBindValue(status);
    query.addBindValue(1); // Default employee ID

    if (query.exec()) {
        QMessageBox::information(this, "Success", "Invoice added successfully!");

        // Create notification for new invoices
        if (status == "Pending") {
            QString notificationMsg = QString("New invoice #%1 created - Amount: %2€ - Due date: %3")
                                          .arg(id)
                                          .arg(QString::number(amount, 'f', 2))
                                          .arg(dueDate.toString("dd/MM/yyyy"));
            notificationManager->createNotification(id, "New invoice", notificationMsg);
        }

        // Refresh the table and clear form
        loadInvoicesFromDatabase();
        clearForm();
        updateNotificationBadge();
        updateVisualStatistics();

    } else {
        QMessageBox::critical(this, "Database Error",
                              "Failed to add invoice: " + query.lastError().text());
    }
}

void MainWindow::editInvoice() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select an invoice to modify.");
        return;
    }

    QString oldId = ui->tableFactures->item(currentRow, 0)->text();
    QString oldStatus = ui->tableFactures->item(currentRow, 4)->text();
    QString amountText = ui->lineMontant->text().trimmed();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString newStatus = ui->comboStatut->currentText();

    // Validation: Amount cannot be empty
    if(amountText.isEmpty()) {
        QMessageBox::warning(this, "Error", "Amount is required!");
        ui->lineMontant->setFocus();
        return;
    }

    // Validation: Amount must be a valid number
    bool ok;
    double amount = amountText.toDouble(&ok);
    if(!ok || amount <= 0) {
        QMessageBox::warning(this, "Error", "Amount must be a valid number greater than 0!");
        ui->lineMontant->setFocus();
        return;
    }

    // Validation: Due date must be after issue date
    if(dueDate <= issueDate) {
        QMessageBox::warning(this, "Error", "Due date must be after issue date!");
        ui->dateEcheance->setFocus();
        return;
    }

    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();
    if (!db.isOpen()) {
        QMessageBox::critical(this, "Database Error", "Database is not connected!");
        return;
    }

    QSqlQuery query(db);
    query.prepare("UPDATE FACTURE SET montant=?, date_emission=?, date_echeance=?, statut=? WHERE id_facture=?");
    query.addBindValue(amount);
    query.addBindValue(issueDate.toString("yyyy-MM-dd"));
    query.addBindValue(dueDate.toString("yyyy-MM-dd"));
    query.addBindValue(newStatus);
    query.addBindValue(oldId);

    if(query.exec()) {
        QMessageBox::information(this, "Success", "Invoice modified successfully!");

        // Notification for status change
        if (oldStatus != newStatus) {
            if (newStatus == "Paid") {
                createPaymentNotification(oldId, amount, QDate::currentDate(), dueDate.addDays(30));
            } else if (newStatus == "Cancelled") {
                createCancellationNotification(oldId, amount, "Sponsor", "Manual modification");
            }
        }

        // Update display
        loadInvoicesFromDatabase();
        updateVisualStatistics();
        clearForm();
        updateNotificationBadge();
    } else {
        QMessageBox::warning(this, "Error", "Failed to modify invoice: " + query.lastError().text());
    }
}

void MainWindow::deleteInvoice() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select an invoice to delete.");
        return;
    }

    QString id = ui->tableFactures->item(currentRow, 0)->text();

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Confirm Deletion",
                                  QString("Are you sure you want to delete invoice '%1'?").arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        // Get database connection
        QSqlDatabase db = Connection::getInstance().getDatabase();
        if (!db.isOpen()) {
            QMessageBox::critical(this, "Database Error", "Database is not connected!");
            return;
        }

        QSqlQuery query(db);
        query.prepare("DELETE FROM FACTURE WHERE id_facture = ?");
        query.addBindValue(id);

        if(query.exec()) {
            QMessageBox::information(this, "Success", "Invoice deleted successfully!");
            loadInvoicesFromDatabase();
            updateVisualStatistics();
            clearForm();
        } else {
            QMessageBox::warning(this, "Error", "Failed to delete invoice: " + query.lastError().text());
        }
    }
}

// =================== HELPER FUNCTIONS ===================
void MainWindow::loadInvoicesFromDatabase() {
    ui->tableFactures->setRowCount(0);

    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();
    if (!db.isOpen()) {
        qDebug() << "Database not connected in loadInvoicesFromDatabase";
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id_facture, montant, date_emission, date_echeance, statut FROM FACTURE ORDER BY id_facture")) {
        qDebug() << "Error loading invoices:" << query.lastError().text();
        return;
    }

    while(query.next()) {
        QString id = query.value(0).toString();
        double amount = query.value(1).toDouble();
        QDate issueDate = query.value(2).toDate();
        QDate dueDate = query.value(3).toDate();
        QString status = query.value(4).toString();

        insertInvoiceInTable(id, amount, issueDate, dueDate, status);
    }

    updateVisualStatistics();
}

void MainWindow::onInvoiceSelected() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow >= 0) {
        // Get data from selected row
        QString id = ui->tableFactures->item(currentRow, 0)->text();
        QString amount = ui->tableFactures->item(currentRow, 1)->text();
        QString issueDateStr = ui->tableFactures->item(currentRow, 2)->text();
        QString dueDateStr = ui->tableFactures->item(currentRow, 3)->text();
        QString status = ui->tableFactures->item(currentRow, 4)->text();

        // Fill the form (ID field is read-only for editing)
        ui->lineId->setText(id);
        ui->lineId->setEnabled(false); // Disable ID field during edit
        ui->lineMontant->setText(amount);

        // Parse dates
        QDate issueDate = QDate::fromString(issueDateStr, "dd/MM/yyyy");
        QDate dueDate = QDate::fromString(dueDateStr, "dd/MM/yyyy");
        if(issueDate.isValid()) ui->dateEmission->setDate(issueDate);
        if(dueDate.isValid()) ui->dateEcheance->setDate(dueDate);

        // Set status
        int index = ui->comboStatut->findText(status);
        if(index >= 0) ui->comboStatut->setCurrentIndex(index);
    }
}

void MainWindow::insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status) {
    int row = ui->tableFactures->rowCount();
    ui->tableFactures->insertRow(row);
    ui->tableFactures->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(amount, 'f', 2)));
    ui->tableFactures->setItem(row, 2, new QTableWidgetItem(issueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 3, new QTableWidgetItem(dueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 4, new QTableWidgetItem(status));
}

void MainWindow::clearForm() {
    ui->lineId->clear();
    ui->lineId->setEnabled(true); // Re-enable ID field for new entries
    ui->lineMontant->clear();
    ui->dateEmission->setDate(QDate::currentDate());
    ui->dateEcheance->setDate(QDate::currentDate().addDays(30)); // Default 30 days later
    ui->comboStatut->setCurrentIndex(0);
}

void MainWindow::sortByAmount() {
    ui->tableFactures->sortItems(1, Qt::AscendingOrder);
}

void MainWindow::searchById() {
    QString searchId = ui->searchBox->text().trimmed();
    if(searchId.isEmpty()) {
        // Show all rows if search is empty
        for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
            ui->tableFactures->setRowHidden(i, false);
        }
        return;
    }

    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        bool match = ui->tableFactures->item(i,0)->text().contains(searchId, Qt::CaseInsensitive);
        ui->tableFactures->setRowHidden(i, !match);
    }
    updateVisualStatistics();
}

void MainWindow::exportToCSV() {
    QString fileName = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV Files (*.csv)");
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);
        stream << "Invoice ID;Amount;Issue Date;Due Date;Status\n";

        for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
            // Only export visible rows (for search functionality)
            if(!ui->tableFactures->isRowHidden(i)) {
                for(int j = 0; j < ui->tableFactures->columnCount(); ++j) {
                    stream << ui->tableFactures->item(i, j)->text();
                    if(j < ui->tableFactures->columnCount() - 1) stream << ";";
                }
                stream << "\n";
            }
        }
        file.close();
        QMessageBox::information(this, "Success", "Data exported successfully to CSV!");
    } else {
        QMessageBox::warning(this, "Error", "Could not export to CSV file.");
    }
}

void MainWindow::toggleDarkTheme() {
    darkTheme = !darkTheme;
    if(darkTheme) {
        applyDarkTheme();
        ui->btnDarkTheme->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->btnDarkTheme->setText("Dark Theme");
    }
}

void MainWindow::initializeVisualStatistics() {
    // Set initial values
    ui->labelTotalValue->setText("0");
    ui->labelAmountValue->setText("$0.00");
    ui->labelPaidValue->setText("0");
    ui->labelPendingValue->setText("0");
    ui->labelCancelledValue->setText("0");
    ui->labelAverageValue->setText("$0.00");

    // Initialize progress bars
    ui->progressPaid->setValue(0);
    ui->progressPending->setValue(0);
    ui->progressCancelled->setValue(0);
}

void MainWindow::updateVisualStatistics() {
    int totalInvoices = 0;
    double totalAmount = 0.0;
    int paidCount = 0;
    int pendingCount = 0;
    int cancelledCount = 0;

    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        // Only count visible rows (for search functionality)
        if(!ui->tableFactures->isRowHidden(i)) {
            totalInvoices++;
            double amount = ui->tableFactures->item(i, 1)->text().toDouble();
            totalAmount += amount;

            QString status = ui->tableFactures->item(i, 4)->text();
            if(status == "Paid") paidCount++;
            else if(status == "Pending") pendingCount++;
            else if(status == "Cancelled") cancelledCount++;
        }
    }

    double averageAmount = totalInvoices > 0 ? totalAmount / totalInvoices : 0.0;

    // Update visual elements
    ui->labelTotalValue->setText(QString::number(totalInvoices));
    ui->labelAmountValue->setText(QString("$%1").arg(QString::number(totalAmount, 'f', 2)));
    ui->labelPaidValue->setText(QString::number(paidCount));
    ui->labelPendingValue->setText(QString::number(pendingCount));
    ui->labelCancelledValue->setText(QString::number(cancelledCount));
    ui->labelAverageValue->setText(QString("$%1").arg(QString::number(averageAmount, 'f', 2)));

    // Update progress bars (percentage of total)
    int totalVisible = totalInvoices;
    if(totalVisible > 0) {
        ui->progressPaid->setValue((paidCount * 100) / totalVisible);
        ui->progressPending->setValue((pendingCount * 100) / totalVisible);
        ui->progressCancelled->setValue((cancelledCount * 100) / totalVisible);
    } else {
        ui->progressPaid->setValue(0);
        ui->progressPending->setValue(0);
        ui->progressCancelled->setValue(0);
    }

    updateCharts();
}

void MainWindow::createStatusPieChart() {
    statusChart->removeAllSeries();

    // Count invoices by status
    int paidCount = 0, pendingCount = 0, cancelledCount = 0;

    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        if(!ui->tableFactures->isRowHidden(i)) {
            QString status = ui->tableFactures->item(i, 4)->text();
            if(status == "Paid") paidCount++;
            else if(status == "Pending") pendingCount++;
            else if(status == "Cancelled") cancelledCount++;
        }
    }

    QPieSeries *series = new QPieSeries();

    if(paidCount > 0) {
        QPieSlice *slice = series->append("Paid", paidCount);
        slice->setColor(QColor("#00AA00"));
        slice->setLabelVisible(true);
        slice->setLabelColor(darkTheme ? Qt::white : Qt::black);
    }

    if(pendingCount > 0) {
        QPieSlice *slice = series->append("Pending", pendingCount);
        slice->setColor(QColor("#FFA500"));
        slice->setLabelVisible(true);
        slice->setLabelColor(darkTheme ? Qt::white : Qt::black);
    }

    if(cancelledCount > 0) {
        QPieSlice *slice = series->append("Cancelled", cancelledCount);
        slice->setColor(QColor("#FF0000"));
        slice->setLabelVisible(true);
        slice->setLabelColor(darkTheme ? Qt::white : Qt::black);
    }

    if(series->count() > 0) {
        statusChart->addSeries(series);
        statusChart->legend()->setVisible(true);
        statusChart->legend()->setAlignment(Qt::AlignBottom);
    }
}

void MainWindow::createTrendLineChart() {
    trendChart->removeAllSeries();

    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();
    if (!db.isOpen()) {
        return;
    }

    // Get monthly data
    QSqlQuery query(db);
    query.prepare("SELECT strftime('%Y-%m', date_emission) as month, "
                  "SUM(montant) as total, COUNT(*) as count "
                  "FROM FACTURE "
                  "GROUP BY strftime('%Y-%m', date_emission) "
                  "ORDER BY month DESC LIMIT 6");

    QLineSeries *amountSeries = new QLineSeries();
    amountSeries->setName("Total Amount (€)");

    QLineSeries *countSeries = new QLineSeries();
    countSeries->setName("Number of invoices");

    QStringList categories;
    double maxAmount = 0;
    int maxCount = 0;

    if(query.exec()) {
        while(query.next()) {
            QString month = query.value(0).toString();
            double total = query.value(1).toDouble();
            int count = query.value(2).toInt();

            categories.prepend(month); // Reverse for chronological order

            amountSeries->append(categories.size() - 1, total);
            countSeries->append(categories.size() - 1, count);

            maxAmount = qMax(maxAmount, total);
            maxCount = qMax(maxCount, count);
        }
    }

    if(amountSeries->count() > 0) {
        trendChart->addSeries(amountSeries);
        trendChart->addSeries(countSeries);

        // X Axis
        QBarCategoryAxis *axisX = new QBarCategoryAxis();
        axisX->append(categories);
        trendChart->addAxis(axisX, Qt::AlignBottom);
        amountSeries->attachAxis(axisX);
        countSeries->attachAxis(axisX);

        // Y Axis for amounts
        QValueAxis *axisY = new QValueAxis();
        axisY->setTitleText("Amount (€)");
        axisY->setRange(0, maxAmount * 1.1);
        trendChart->addAxis(axisY, Qt::AlignLeft);
        amountSeries->attachAxis(axisY);

        // Secondary Y Axis for counts
        QValueAxis *axisY2 = new QValueAxis();
        axisY2->setTitleText("Number of invoices");
        axisY2->setRange(0, maxCount * 1.1);
        trendChart->addAxis(axisY2, Qt::AlignRight);
        countSeries->attachAxis(axisY2);
    }
}

void MainWindow::updateCharts() {
    createStatusPieChart();
    createTrendLineChart();
}

void MainWindow::updateAnalytics() {
    if(predictionEngine) {
        auto prediction = predictionEngine->predictNextMonthRevenue();
        auto probabilities = predictionEngine->getPaymentProbability();
        auto health = predictionEngine->analyzeFinancialHealth();

        ui->labelPredictionValue->setText(QString("%1 €").arg(prediction.predictedAmount, 0, 'f', 2));
        ui->labelConfidenceValue->setText(QString("%1%").arg(prediction.confidence * 100, 0, 'f', 1));
        ui->labelTrendValue->setText(prediction.trend);
        ui->labelRecommendationValue->setText(prediction.recommendation);
        ui->labelHealthValue->setText(health.status);

        ui->listProbabilities->clear();
        for(const auto& prob : probabilities) {
            ui->listProbabilities->addItem(QString("%1: %2%").arg(prob.first).arg(prob.second, 0, 'f', 1));
        }

        // Update charts in layout
        QLayoutItem* item;
        while ((item = ui->verticalLayout_5->takeAt(0)) != nullptr) {
            delete item->widget();
            delete item;
        }

        ui->verticalLayout_5->addWidget(statusChartView);
        ui->verticalLayout_5->addWidget(trendChartView);
    }
}

void MainWindow::applyLightTheme() {
    this->setStyleSheet(R"(
        QMainWindow { background-color: #FFFFFF; border-radius: 10px; }
        QGroupBox { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; font: bold 14pt "Arial"; color: #7D4FEE; margin: 10px; }
        QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
        QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
        QLineEdit, QComboBox, QDateEdit { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; font: 10pt "Arial"; color: #7D4FEE; }
        QTableWidget { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; gridline-color: #7D4FEE; font: 10pt "Arial"; color: #7D4FEE; }
        QLabel { color: #7D4FEE; font: 10pt "Arial"; }
        QFrame { background-color: #F8F9FA; border: 1px solid #E0E0E0; border-radius: 8px; padding: 10px; }
        QProgressBar { border: 1px solid #7D4FEE; border-radius: 4px; text-align: center; background-color: #FFFFFF; }
        QProgressBar::chunk { border-radius: 3px; }
        QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
        QListWidget { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; font: 10pt "Arial"; color: #7D4FEE; }
    )");

    if (statusChart) statusChart->setTheme(QChart::ChartThemeLight);
    if (trendChart) trendChart->setTheme(QChart::ChartThemeLight);
}

void MainWindow::applyDarkTheme() {
    this->setStyleSheet(R"(
        QMainWindow { background-color: #000000; }
        QGroupBox { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QLineEdit, QComboBox, QDateEdit { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QTableWidget { background-color: #1A1A1A; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QLabel { color: #7D4FEE; }
        QFrame { background-color: #2D2D2D; border: 1px solid #7D4FEE; border-radius: 8px; padding: 10px; }
        QProgressBar { border: 1px solid #7D4FEE; border-radius: 4px; text-align: center; background-color: #1A1A1A; color: white; }
        QProgressBar::chunk { border-radius: 3px; }
        QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
        QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
        QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
        QListWidget { background-color: #1A1A1A; border: 2px solid #7D4FEE; color: #7D4FEE; }
    )");

    if (statusChart) statusChart->setTheme(QChart::ChartThemeDark);
    if (trendChart) trendChart->setTheme(QChart::ChartThemeDark);
}

// =================== NOTIFICATIONS FUNCTIONS ===================
void MainWindow::updateNotificationBadge() {
    int unreadCount = notificationManager->getUnreadCount();
    ui->btnNotifications->setText(unreadCount > 0 ?
                                      QString("🔔 Notifications (%1)").arg(unreadCount) : "🔔 Notifications");
}

void MainWindow::loadNotifications() {
    ui->listNotifications->clear();

    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();
    if (!db.isOpen()) {
        return;
    }

    QSqlQuery query(db);
    if (!query.exec("SELECT id_notification, id_facture, type_alerte, message, date_creation, statut "
                    "FROM Notifications ORDER BY date_creation DESC")) {
        qDebug() << "Error loading notifications:" << query.lastError().text();
        return;
    }

    while (query.next()) {
        int id = query.value(0).toInt();
        QString invoiceId = query.value(1).toString();
        QString type = query.value(2).toString();
        QString message = query.value(3).toString();
        QString date = query.value(4).toDateTime().toString("dd/MM/yyyy HH:mm");
        QString status = query.value(5).toString();

        QString itemText = QString("[%1] %2 - %3\n%4")
                               .arg(date)
                               .arg(type)
                               .arg(message)
                               .arg(status == "Non lu" ? "🔴 UNREAD" : "✅ READ");

        QListWidgetItem *item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, id);

        if (status == "Non lu") {
            item->setBackground(QColor(255, 235, 235)); // Light red for unread
        }

        ui->listNotifications->addItem(item);
    }
}

void MainWindow::markAllAsRead() {
    // Get database connection
    QSqlDatabase db = Connection::getInstance().getDatabase();
    if (!db.isOpen()) {
        return;
    }

    QSqlQuery query(db);
    if (query.exec("UPDATE Notifications SET statut = 'Lu' WHERE statut = 'Non lu'")) {
        loadNotifications();
        updateNotificationBadge();
        QMessageBox::information(this, "Success", "All notifications marked as read");
    }
}

void MainWindow::onNotificationClicked() {
    QListWidgetItem *item = ui->listNotifications->currentItem();
    if (item) {
        int notificationId = item->data(Qt::UserRole).toInt();
        notificationManager->markAsRead(notificationId);
        item->setBackground(QColor(235, 255, 235)); // Light green for read
        updateNotificationBadge();
    }
}

void MainWindow::createPaymentNotification(const QString &invoiceId, double amount, const QDate &paymentDate, const QDate &nextDue) {
    QString message = QString("✅ PAYMENT RECEIVED #%1\n"
                              "Amount: %2€ - Date: %3\n"
                              "Next due date: %4\n"
                              "Thank you for your punctuality! 🎉")
                          .arg(invoiceId)
                          .arg(QString::number(amount, 'f', 2))
                          .arg(paymentDate.toString("dd/MM/yyyy"))
                          .arg(nextDue.toString("dd/MM/yyyy"));

    notificationManager->createNotification(invoiceId, "Payment received", message);
    updateNotificationBadge();
}

void MainWindow::createCancellationNotification(const QString &invoiceId, double amount, const QString &sponsor, const QString &reason) {
    QString message = QString("❌ CANCELLATION CONFIRMED\n\n"
                              "Invoice #%1 cancelled\n"
                              "Amount: %2€ - Sponsor: %3\n"
                              "Reason: %4\n\n"
                              "✅ Status: Refund initiated\n"
                              "⏱ Delay: 5-7 business days\n"
                              "📞 Contact: 01 23 45 67 89\n\n"
                              "Thank you for your trust.")
                          .arg(invoiceId)
                          .arg(QString::number(amount, 'f', 2))
                          .arg(sponsor)
                          .arg(reason);

    notificationManager->createNotification(invoiceId, "Cancellation", message);
    updateNotificationBadge();
}

MainWindow::~MainWindow() {
    delete ui;
    delete notificationManager;
    delete predictionEngine;
}
