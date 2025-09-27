#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>

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
    connect(ui->btnAdd, &QPushButton::clicked, this, &MainWindow::addFacture);
    connect(ui->btnEdit, &QPushButton::clicked, this, &MainWindow::editFacture);
    connect(ui->btnDelete, &QPushButton::clicked, this, &MainWindow::deleteFacture);
    connect(ui->btnClear, &QPushButton::clicked, this, &MainWindow::clearForm);
    connect(ui->btnSortByMontant, &QPushButton::clicked, this, &MainWindow::sortByMontant);
    connect(ui->btnSearchById, &QPushButton::clicked, this, &MainWindow::searchById);

    // =================== Application Style ===================
    this->setStyleSheet(R"(
        QPushButton {
            background-color: #6c63ff;
            color: white;
            border-radius: 8px;
            padding: 6px 12px;
            font-weight: bold;
        }
        QPushButton:hover {
            background-color: #5a54d1;
        }
        QLineEdit, QDateEdit, QComboBox {
            border: 1px solid #ccc;
            border-radius: 5px;
            padding: 4px;
            background: #f9f9f9;
        }
        QTableWidget {
            border: 1px solid #ccc;
            gridline-color: #ddd;
            selection-background-color: #6c63ff;
            selection-color: white;
        }
        QGroupBox {
            font-weight: bold;
            border: 1px solid #aaa;
            border-radius: 6px;
            margin-top: 10px;
        }
        QGroupBox::title {
            subcontrol-origin: margin;
            subcontrol-position: top left;
            padding: 0 5px;
        }
    )");
}

// =================== Menu Page Functions ===================
void MainWindow::showManagementPage() { ui->stackedWidget->setCurrentWidget(ui->managementPage); }
void MainWindow::showFinancePage() { ui->stackedWidget->setCurrentWidget(ui->financePage); }
void MainWindow::showContentCreatorPage() { ui->stackedWidget->setCurrentWidget(ui->contentCreatorPage); }
void MainWindow::showSponsorPage() { ui->stackedWidget->setCurrentWidget(ui->sponsorPage); }
void MainWindow::showEmployeePage() { ui->stackedWidget->setCurrentWidget(ui->employeePage); }

// =================== Finance Page Functions ===================
void MainWindow::insertFactureInTable(QString id, double montant, QDate dateEmission, QDate dateEcheance, QString statut)
{
    int row = ui->tableFactures->rowCount();
    ui->tableFactures->insertRow(row);
    ui->tableFactures->setItem(row, 0, new QTableWidgetItem(id));
    ui->tableFactures->setItem(row, 1, new QTableWidgetItem(QString::number(montant)));
    ui->tableFactures->setItem(row, 2, new QTableWidgetItem(dateEmission.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 3, new QTableWidgetItem(dateEcheance.toString("dd/MM/yyyy")));
    ui->tableFactures->setItem(row, 4, new QTableWidgetItem(statut));
}

void MainWindow::addFacture()
{
    QString id = ui->lineId->text();
    double montant = ui->lineMontant->text().toDouble();
    QDate dateEmission = ui->dateEmission->date();
    QDate dateEcheance = ui->dateEcheance->date();
    QString statut = ui->comboStatut->currentText();

    if(id.isEmpty() || montant <= 0) {
        QMessageBox::warning(this, "Erreur", "ID et Montant obligatoires !");
        return;
    }

    insertFactureInTable(id, montant, dateEmission, dateEcheance, statut);
    clearForm();
}

void MainWindow::editFacture()
{
    int row = ui->tableFactures->currentRow();
    if(row < 0) {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une facture à éditer.");
        return;
    }

    ui->tableFactures->item(row, 0)->setText(ui->lineId->text());
    ui->tableFactures->item(row, 1)->setText(ui->lineMontant->text());
    ui->tableFactures->item(row, 2)->setText(ui->dateEmission->date().toString("dd/MM/yyyy"));
    ui->tableFactures->item(row, 3)->setText(ui->dateEcheance->date().toString("dd/MM/yyyy"));
    ui->tableFactures->item(row, 4)->setText(ui->comboStatut->currentText());
}

void MainWindow::deleteFacture()
{
    int row = ui->tableFactures->currentRow();
    if(row >= 0) {
        ui->tableFactures->removeRow(row);
    } else {
        QMessageBox::warning(this, "Erreur", "Veuillez sélectionner une facture à supprimer.");
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

void MainWindow::sortByMontant()
{
    ui->tableFactures->sortItems(1, Qt::AscendingOrder); // colonne Montant
}

void MainWindow::searchById()
{
    QString searchId = ui->searchBox->text();
    for(int i = 0; i < ui->tableFactures->rowCount(); ++i) {
        ui->tableFactures->setRowHidden(i, ui->tableFactures->item(i,0)->text() != searchId);
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}
