#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>

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

    // Apply initial style
    applyLightTheme();
}

void MainWindow::applyLightTheme()
{
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #FFFFFF;
            border-radius: 10px;
        }
        QGroupBox {
            background-color: #FFFFFF;
            border: 2px solid #7D4FEE;
            border-radius: 10px;
            font: bold 14pt "Arial";
            color: #7D4FEE;
            margin: 10px;
        }
        QGroupBox::title {
            color: #7D4FEE;
            padding: 0 5px;
            subcontrol-origin: margin;
            subcontrol-position: top left;
        }
        QPushButton {
            background-color: #7D4FEE;
            color: #FFFFFF;
            border-radius: 10px;
            padding: 8px;
            min-width: 100px;
            font: 10pt "Arial";
            border: 2px solid #7D4FEE;
        }
        QPushButton:hover {
            background-color: #FFFFFF;
            color: #7D4FEE;
            border: 2px solid #7D4FEE;
        }
        QLineEdit, QComboBox, QDateEdit {
            background-color: #FFFFFF;
            border: 2px solid #7D4FEE;
            border-radius: 10px;
            padding: 6px;
            font: 10pt "Arial";
            color: #7D4FEE;
        }
        QTableWidget {
            background-color: #FFFFFF;
            border: 2px solid #7D4FEE;
            border-radius: 10px;
            gridline-color: #7D4FEE;
            font: 10pt "Arial";
            color: #7D4FEE;
        }
        QTableWidget::item:selected {
            background-color: #7D4FEE;
            color: #FFFFFF;
        }
        QLabel {
            color: #7D4FEE;
            font: 10pt "Arial";
        }
        QWidget#menuFrame {
            background-color: #7D4FEE;
            border-radius: 10px 0 0 10px;
        }
        QPushButton#sidebarButton {
            background-color: #7D4FEE;
            color: #FFFFFF;
            border: none;
            border-radius: 10px;
            padding: 10px;
            font: 10pt "Arial";
            width: 100%;
            text-align: left;
        }
        QPushButton#sidebarButton:hover {
            background-color: #FFFFFF;
            color: #7D4FEE;
            border: 2px solid #7D4FEE;
        }
    )");
}

void MainWindow::applyDarkTheme()
{
    this->setStyleSheet(R"(
        QMainWindow {
            background-color: #000000;
        }
        QGroupBox {
            background-color: #000000;
            border: 2px solid #7D4FEE;
            color: #7D4FEE;
        }
        QLineEdit, QComboBox, QDateEdit {
            background-color: #000000;
            border: 2px solid #7D4FEE;
            color: #7D4FEE;
        }
        QTableWidget {
            background-color: #1A1A1A;
            border: 2px solid #7D4FEE;
            color: #7D4FEE;
        }
        QLabel {
            color: #7D4FEE;
        }
        QPushButton {
            background-color: #7D4FEE;
            color: #FFFFFF;
            border-radius: 10px;
            padding: 8px;
            min-width: 100px;
            font: 10pt "Arial";
            border: 2px solid #7D4FEE;
        }
        QPushButton:hover {
            background-color: #FFFFFF;
            color: #7D4FEE;
            border: 2px solid #7D4FEE;
        }
        QWidget#menuFrame {
            background-color: #7D4FEE;
            border-radius: 10px 0 0 10px;
        }
        QPushButton#sidebarButton {
            background-color: #7D4FEE;
            color: #FFFFFF;
            border: none;
            border-radius: 10px;
            padding: 10px;
            font: 10pt "Arial";
            width: 100%;
            text-align: left;
        }
        QPushButton#sidebarButton:hover {
            background-color: #FFFFFF;
            color: #7D4FEE;
            border: 2px solid #7D4FEE;
        }
    )");
}

void MainWindow::initializeStatisticsTable()
{
    // Configure statistics table
    ui->tableStats->setRowCount(6);
    ui->tableStats->setColumnCount(2);

    // Set headers
    QStringList headers;
    headers << "Metric" << "Value";
    ui->tableStats->setHorizontalHeaderLabels(headers);

    // Set rows
    QStringList metrics;
    metrics << "Total Invoices" << "Total Amount" << "Paid Invoices"
            << "Pending Invoices" << "Cancelled Invoices" << "Average Amount";

    for(int i = 0; i < metrics.size(); ++i) {
        ui->tableStats->setItem(i, 0, new QTableWidgetItem(metrics[i]));
        ui->tableStats->setItem(i, 1, new QTableWidgetItem("0"));
    }

    // Make the table read-only
    ui->tableStats->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void MainWindow::updateStatistics()
{
    int totalInvoices = ui->tableFactures->rowCount();
    double totalAmount = 0.0;
    int paidCount = 0;
    int pendingCount = 0;
    int cancelledCount = 0;

    // Calculate statistics
    for(int i = 0; i < totalInvoices; ++i) {
        double amount = ui->tableFactures->item(i, 1)->text().toDouble();
        totalAmount += amount;

        QString status = ui->tableFactures->item(i, 4)->text();
        if(status == "Paid") paidCount++;
        else if(status == "Pending") pendingCount++;
        else if(status == "Cancelled") cancelledCount++;
    }

    double averageAmount = totalInvoices > 0 ? totalAmount / totalInvoices : 0.0;

    // Update statistics table
    ui->tableStats->item(0, 1)->setText(QString::number(totalInvoices));
    ui->tableStats->item(1, 1)->setText(QString::number(totalAmount, 'f', 2));
    ui->tableStats->item(2, 1)->setText(QString::number(paidCount));
    ui->tableStats->item(3, 1)->setText(QString::number(pendingCount));
    ui->tableStats->item(4, 1)->setText(QString::number(cancelledCount));
    ui->tableStats->item(5, 1)->setText(QString::number(averageAmount, 'f', 2));
}

// =================== Menu Page Functions ===================
void MainWindow::showManagementPage() { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showFinancePage() { ui->stackedWidget->setCurrentWidget(ui->financePage); }
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage() { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showEmployeePage() { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// =================== Finance Page Functions ===================
void MainWindow::insertInvoiceInTable(QString id, double amount, QDate issueDate, QDate dueDate, QString status)
{
    int row = ui->tableFactures->rowCount();
    ui->tableFactures->insertRow(row);
    ui->tableFactures->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(amount)));
    ui->tableFactures->setItem(row, 2, new QTableWidgetItem(issueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 3, new QTableWidgetItem(dueDate.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 4, new QTableWidgetItem(status));

    // Update statistics after adding invoice
    updateStatistics();
}

void MainWindow::addInvoice()
{
    QString id = ui->lineId->text();
    double amount = ui->lineMontant->text().toDouble();
    QDate issueDate = ui->dateEmission->date();
    QDate dueDate = ui->dateEcheance->date();
    QString status = ui->comboStatut->currentText();

    if(id.isEmpty() || amount <= 0) {
        QMessageBox::warning(this, "Error", "Invoice ID and Amount are required!");
        return;
    }

    insertInvoiceInTable(id, amount, issueDate, dueDate, status);
    clearForm();
}

void MainWindow::editInvoice()
{
    int row = ui->tableFactures->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Error", "Please select an invoice to edit.");
        return;
    }

    ui->tableFactures->item(row, 0)->setText(ui->lineId->text());
    ui->tableFactures->item(row, 1)->setText(ui->lineMontant->text());
    ui->tableFactures->item(row, 2)->setText(ui->dateEmission->date().toString("dd/MM/yyyy"));
    ui->tableFactures->item(row, 3)->setText(ui->dateEcheance->date().toString("dd/MM/yyyy"));
    ui->tableFactures->item(row, 4)->setText(ui->comboStatut->currentText());

    // Update statistics after editing invoice
    updateStatistics();
}

void MainWindow::deleteInvoice()
{
    int row = ui->tableFactures->currentRow();
    if(row >= 0) {
        ui->tableFactures->removeRow(row);
        // Update statistics after deleting invoice
        updateStatistics();
    } else {
        QMessageBox::warning(this, "Error", "Please select an invoice to delete.");
    }
}

void MainWindow::clearForm()
{
    ui->lineId->clear();
    ui->lineMontant->clear();
    ui->dateEmission->setDate(QDate::currentDate());
    ui->dateEcheance->setDate(QDate::currentDate());
    ui->comboStatut->setCurrentIndex(0);
}

void MainWindow::sortByAmount()
{
    ui->tableFactures->sortItems(1, Qt::AscendingOrder); // Amount column
}

void MainWindow::searchById()
{
    QString searchId = ui->searchBox->text();
    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        ui->tableFactures->setRowHidden(i, ui->tableFactures->item(i,0)->text() != searchId);
    }
}

void MainWindow::exportToCSV()
{
    QString fileName = QFileDialog::getSaveFileName(this, "Export to CSV", "", "CSV Files (*.csv)");
    if(fileName.isEmpty()) return;

    QFile file(fileName);
    if(file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream stream(&file);

        // Headers
        stream << "Invoice ID;Amount;Issue Date;Due Date;Status\n";

        // Data
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

void MainWindow::toggleDarkTheme()
{
    darkTheme = !darkTheme;
    if(darkTheme) {
        applyDarkTheme();
        ui->btnDarkTheme->setText("Light Theme");
    } else {
        applyLightTheme();
        ui->btnDarkTheme->setText("Dark Theme");
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
