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

    // Initialize statistics table
    initializeStatisticsTable();
    applyLightTheme();
}

// =================== FONCTIONS MENU ===================
void MainWindow::showManagementPage() { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showFinancePage() { ui->stackedWidget->setCurrentWidget(ui->financePage); }
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage() { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showEmployeePage() { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// =================== FONCTIONS FINANCE ===================
void MainWindow::addInvoice() {
    QString id = ui->lineId->text();
    double amount = ui->lineMontant->text().toDouble();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    if(id.isEmpty() || amount <= 0) {
        QMessageBox::warning(this, "Erreur", "ID et Montant sont obligatoires!");
        return;
    }

    // ✅ TEST AVEC LA BASE DE DONNÉES
    QSqlQuery query;
    query.prepare("INSERT INTO Facture (id_facture, montant, date_emission, date_echeance, statut, id_employe) "
                  "VALUES (:id, :montant, :date_emission, :date_echeance, :statut, :id_employe)");

    query.bindValue(":id", id);
    query.bindValue(":montant", amount);
    query.bindValue(":date_emission", issueDate);
    query.bindValue(":date_echeance", dueDate);
    query.bindValue(":statut", status);
    query.bindValue(":id_employe", 1); // Valeur par défaut

    if(query.exec()) {
        QMessageBox::information(this, "Succès", "Facture ajoutée à la base Oracle!");
        insertInvoiceInTable(id, amount, issueDate, dueDate, status);
        clearForm();
    } else {
        QMessageBox::warning(this, "Erreur BD", "Échec d'ajout: " + query.lastError().text());
    }
}

void MainWindow::editInvoice() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une facture à modifier.");
        return;
    }

    QString oldId = ui->tableFactures->item(currentRow, 0)->text();
    QString newId = ui->lineId->text();
    double amount = ui->lineMontant->text().toDouble();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    // ✅ MISE À JOUR DANS LA BASE
    QSqlQuery query;
    query.prepare("UPDATE Facture SET id_facture=:new_id, montant=:montant, "
                  "date_emission=:date_emission, date_echeance=:date_echeance, statut=:statut "
                  "WHERE id_facture=:old_id");

    query.bindValue(":new_id", newId);
    query.bindValue(":montant", amount);
    query.bindValue(":date_emission", issueDate);
    query.bindValue(":date_echeance", dueDate);
    query.bindValue(":statut", status);
    query.bindValue(":old_id", oldId);

    if(query.exec()) {
        QMessageBox::information(this, "Succès", "Facture modifiée dans Oracle!");
        // Mettre à jour l'affichage
        ui->tableFactures->item(currentRow, 0)->setText(newId);
        ui->tableFactures->item(currentRow, 1)->setText(QString::number(amount));
        ui->tableFactures->item(currentRow, 2)->setText(issueDate.toString("dd/MM/yyyy"));
        ui->tableFactures->item(currentRow, 3)->setText(dueDate.toString("dd/MM/yyyy"));
        ui->tableFactures->item(currentRow, 4)->setText(status);
        clearForm();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec modification: " + query.lastError().text());
    }
}

void MainWindow::deleteInvoice() {
    int currentRow = ui->tableFactures->currentRow();
    if(currentRow < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une facture à supprimer.");
        return;
    }

    QString id = ui->tableFactures->item(currentRow, 0)->text();

    // ✅ SUPPRESSION DANS LA BASE
    QSqlQuery query;
    query.prepare("DELETE FROM Facture WHERE id_facture = :id");
    query.bindValue(":id", id);

    if(query.exec()) {
        QMessageBox::information(this, "Succès", "Facture supprimée d'Oracle!");
        ui->tableFactures->removeRow(currentRow);
        updateStatistics();
    } else {
        QMessageBox::warning(this, "Erreur", "Échec suppression: " + query.lastError().text());
    }
}

// =================== FONCTIONS EXISTANTES ===================
void MainWindow::insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status) {
    int row = ui->tableFactures->rowCount();
    ui->tableFactures->insertRow(row);
    ui->tableFactures->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
    ui->tableFactures->setItem(row, 2, new QTableWidgetItem(issueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 3, new QTableWidgetItem(dueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 4, new QTableWidgetItem(status));
    updateStatistics();
}

void MainWindow::clearForm() {
    ui->lineId->clear();
    ui->lineMontant->clear();
    ui->dateEmission->setDate(QDate::currentDate());
    ui->dateEcheance->setDate(QDate::currentDate());
    ui->comboStatut->setCurrentIndex(0);
}

void MainWindow::sortByAmount() {
    ui->tableFactures->sortItems(1, Qt::AscendingOrder);
}

void MainWindow::searchById() {
    QString searchId = ui->searchBox->text();
    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        ui->tableFactures->setRowHidden(i, ui->tableFactures->item(i,0)->text() != searchId);
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
            for(int j = 0; j < ui->tableFactures->columnCount(); ++j) {
                stream << ui->tableFactures->item(i, j)->text();
                if(j < ui->tableFactures->columnCount() - 1) stream << ";";
            }
            stream << "\n";
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

void MainWindow::updateStatistics() {
    int totalInvoices = ui->tableFactures->rowCount();
    double totalAmount = 0.0;
    int paidCount = 0;
    int pendingCount = 0;
    int cancelledCount = 0;

    for(int i = 0; i < totalInvoices; ++i) {
        double amount = ui->tableFactures->item(i, 1)->text().toDouble();
        totalAmount += amount;

        QString status = ui->tableFactures->item(i, 4)->text();
        if(status == "Paid") paidCount++;
        else if(status == "Pending") pendingCount++;
        else if(status == "Cancelled") cancelledCount++;
    }

    double averageAmount = totalInvoices > 0 ? totalAmount / totalInvoices : 0.0;

    ui->tableStats->item(0, 1)->setText(QString::number(totalInvoices));
    ui->tableStats->item(1, 1)->setText(QString::number(totalAmount, 'f', 2));
    ui->tableStats->item(2, 1)->setText(QString::number(paidCount));
    ui->tableStats->item(3, 1)->setText(QString::number(pendingCount));
    ui->tableStats->item(4, 1)->setText(QString::number(cancelledCount));
    ui->tableStats->item(5, 1)->setText(QString::number(averageAmount, 'f', 2));
}

void MainWindow::applyLightTheme() {
    // Votre code existant pour le thème clair
    this->setStyleSheet(R"(
        QMainWindow { background-color: #FFFFFF; border-radius: 10px; }
        QGroupBox { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; font: bold 14pt "Arial"; color: #7D4FEE; margin: 10px; }
        QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
        QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
        QLineEdit, QComboBox, QDateEdit { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; padding: 6px; font: 10pt "Arial"; color: #7D4FEE; }
        QTableWidget { background-color: #FFFFFF; border: 2px solid #7D4FEE; border-radius: 10px; gridline-color: #7D4FEE; font: 10pt "Arial"; color: #7D4FEE; }
        QLabel { color: #7D4FEE; font: 10pt "Arial"; }
        QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
    )");
}

void MainWindow::applyDarkTheme() {
    // Votre code existant pour le thème sombre
    this->setStyleSheet(R"(
        QMainWindow { background-color: #000000; }
        QGroupBox { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QLineEdit, QComboBox, QDateEdit { background-color: #000000; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QTableWidget { background-color: #1A1A1A; border: 2px solid #7D4FEE; color: #7D4FEE; }
        QLabel { color: #7D4FEE; }
        QPushButton { background-color: #7D4FEE; color: #FFFFFF; border-radius: 10px; padding: 8px; min-width: 100px; font: 10pt "Arial"; border: 2px solid #7D4FEE; }
        QPushButton:hover { background-color: #FFFFFF; color: #7D4FEE; border: 2px solid #7D4FEE; }
        QWidget#menuFrame { background-color: #7D4FEE; border-radius: 10px 0 0 10px; }
    )");
}

void MainWindow::initializeStatisticsTable() {
    ui->tableStats->setRowCount(6);
    ui->tableStats->setColumnCount(2);

    QStringList headers;
    headers << "Metric" << "Value";
    ui->tableStats->setHorizontalHeaderLabels(headers);

    QStringList metrics;
    metrics << "Total Invoices" << "Total Amount" << "Paid Invoices"
            << "Pending Invoices" << "Cancelled Invoices" << "Average Amount";

    for(int i = 0; i < metrics.size(); ++i) {
        ui->tableStats->setItem(i, 0, new QTableWidgetItem(metrics[i]));
        ui->tableStats->setItem(i, 1, new QTableWidgetItem("0"));
    }

    ui->tableStats->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}
