#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // =================== Menu Buttons ===================
    connect(ui->btnManagement, &QPushButton::clicked, this, &MainWindow::showManagementPage);
    connect(ui->btnFinance, &QPushButton::clicked, this, &MainWindow::showFinancePage);
    connect(ui->btnContentCreator, &QPushButton::clicked, this, &MainWindow::showContentCreatorPage);
    connect(ui->btnSponsor, &QPushButton::clicked, this, &MainWindow::showSponsorPage);
    connect(ui->btnEmployee, &QPushButton::clicked, this, &MainWindow::showEmployeePage);

    // =================== Finance Buttons ===================
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addInvoice);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editInvoice);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteInvoice);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(ui->btnSortByMontant, &QPushButton::clicked, this, &MainWindow::sortByAmount);
    connect(ui->btnSearchById, &QPushButton::clicked, this, &MainWindow::searchById);
    connect(ui->btnExportCSV, &QPushButton::clicked, this, &MainWindow::exportToCSV);
    connect(ui->btnDarkTheme, &QPushButton::clicked, this, &MainWindow::toggleDarkTheme);

    // Connect table selection to auto-fill form
    connect(ui->tableFactures, &QTableWidget::itemSelectionChanged, this, &MainWindow::onInvoiceSelected);

    // Initialize and load data
    initializeVisualStatistics();
    loadInvoicesFromDatabase();
    applyLightTheme();
}

// =================== MENU FUNCTIONS ===================
void MainWindow::showManagementPage() { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showFinancePage() {
    ui->stackedWidget->setCurrentWidget(ui->financePage);
    loadInvoicesFromDatabase();
}
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage() { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showEmployeePage() { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// =================== FINANCE FUNCTIONS ===================
void MainWindow::addInvoice() {
    QString id = ui->lineId->text().trimmed();
    QString amountText = ui->lineMontant->text().trimmed();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    // Validation: Invoice ID cannot be empty
    if(id.isEmpty()) {
        QMessageBox::warning(this, "Error", "Invoice ID is required!");
        ui->lineId->setFocus();
        return;
    }

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

    // Check if invoice ID already exists
    QSqlQuery checkQuery;
    checkQuery.prepare("SELECT COUNT(*) FROM Facture WHERE id_facture = :id");
    checkQuery.bindValue(":id", id);
    if(checkQuery.exec() && checkQuery.next()) {
        if(checkQuery.value(0).toInt() > 0) {
            QMessageBox::warning(this, "Error",
                                 QString("Invoice ID '%1' already exists! Please use a different ID.").arg(id));
            ui->lineId->setFocus();
            return;
        }
    }

    QSqlQuery query;
    query.prepare("INSERT INTO Facture (id_facture, montant, date_emission, date_echeance, statut, id_employe) "
                  "VALUES (:id, :montant, :date_emission, :date_echeance, :statut, :id_employe)");

    query.bindValue(":id", id);
    query.bindValue(":montant", amount);
    query.bindValue(":date_emission", issueDate);
    query.bindValue(":date_echeance", dueDate);
    query.bindValue(":statut", status);
    query.bindValue(":id_employe", 1); // Default value

    if(query.exec()) {
        QMessageBox::information(this, "Success", "Invoice added successfully!");
        insertInvoiceInTable(id, amount, issueDate, dueDate, status);
        clearForm();
    } else {
        QMessageBox::warning(this, "Database Error", "Failed to add invoice: " + query.lastError().text());
    }
}

void MainWindow::editInvoice() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow < 0) {
        QMessageBox::warning(this, "Error", "Please select an invoice to modify.");
        return;
    }

    QString oldId = ui->tableFactures->item(currentRow, 0)->text();
    QString amountText = ui->lineMontant->text().trimmed();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

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

    QSqlQuery query;
    query.prepare("UPDATE Facture SET montant=:montant, "
                  "date_emission=:date_emission, date_echeance=:date_echeance, statut=:statut "
                  "WHERE id_facture=:old_id");

    query.bindValue(":montant", amount);
    query.bindValue(":date_emission", issueDate);
    query.bindValue(":date_echeance", dueDate);
    query.bindValue(":statut", status);
    query.bindValue(":old_id", oldId);

    if(query.exec()) {
        QMessageBox::information(this, "Success", "Invoice modified successfully!");
        // Update display (ID remains the same)
        ui->tableFactures->item(currentRow, 1)->setText(QString::number(amount, 'f', 2));
        ui->tableFactures->item(currentRow, 2)->setText(issueDate.toString("dd/MM/yyyy"));
        ui->tableFactures->item(currentRow, 3)->setText(dueDate.toString("dd/MM/yyyy"));
        ui->tableFactures->item(currentRow, 4)->setText(status);
        updateVisualStatistics();
        clearForm();
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
    reply = QMessageBox::question(this, "Confirm Delete",
                                  QString("Are you sure you want to delete invoice '%1'?").arg(id),
                                  QMessageBox::Yes | QMessageBox::No);

    if(reply == QMessageBox::Yes) {
        QSqlQuery query;
        query.prepare("DELETE FROM Facture WHERE id_facture = :id");
        query.bindValue(":id", id);

        if(query.exec()) {
            QMessageBox::information(this, "Success", "Invoice deleted successfully!");
            ui->tableFactures->removeRow(currentRow);
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

    QSqlQuery query("SELECT id_facture, montant, date_emission, date_echeance, statut FROM Facture ORDER BY id_facture");

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
    )");
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
    )");
}

MainWindow::~MainWindow()
{
    delete ui;
}
