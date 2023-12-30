create database VehiculeDB;
use VehiculeDB;
CREATE TABLE Invoices(
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(255),
    montant FLOAT
);
INSERT INTO Invoices (nom, montant) VALUES ('John Doe', 25500.500),
('Jane ', 30000);
