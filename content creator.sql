CREATE TABLE Plateforme (
    ID NUMBER PRIMARY KEY,
    Nom VARCHAR2(50) NOT NULL,
    URL VARCHAR2(100)
);

CREATE TABLE Créateur (
    ID NUMBER PRIMARY KEY,
    Nom VARCHAR2(100) NOT NULL,
    PlateformeID NUMBER REFERENCES Plateforme(ID),
    Abonnés NUMBER,
    TypeContenu VARCHAR2(50)
);

CREATE TABLE Contenu (
    ID NUMBER PRIMARY KEY,
    Type VARCHAR2(50),
    DateCreation DATE,
    CréateurID NUMBER REFERENCES Créateur(ID)
);

-- Insertion exemple
INSERT INTO Plateforme VALUES (1, 'YouTube', 'youtube.com');
INSERT INTO Créateur VALUES (1, 'John Doe', 1, 100000, 'Gaming');