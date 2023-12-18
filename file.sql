create database socketdb;
use socketdb;
CREATE TABLE votre_table (
    id INT AUTO_INCREMENT PRIMARY KEY,
    nom VARCHAR(255),
    montant FLOAT
);
INSERT INTO votre_table (nom, montant) VALUES ('John Doe', 25500.500),
('Jane ', 30000);
