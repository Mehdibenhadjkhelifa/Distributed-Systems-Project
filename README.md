# Distributed systems Project
this is an assignment for my university 

**STATUS:** *Work In Progress*

## Dependencies
For this project to be setup and run you should have:

### 1- A mysql databse

download a mysql database like ``Mysql`` ``MariaDB`` .

### 2- Have Gcc compiler

download a gcc compiler to compile provided code with provided shell script

### 3- Mysql Library

download the mysql library to staticly link it with the ``ServeurTCP`` binary

## Setup

### 1- Clone Repository
clone the repository locally and cd into it with the following commands:
```bash
git clone https://github.com/Mehdibenhadjkhelifa/projet_sr.git
cd projet_sr
```
### 2- download dependencies

you need download all dependencies listed above 

**OR**

if you are in a ubuntu system run:
```bash
scripts/ubuntuSetup.sh
```

### 3- Setup MySql Database

#### a- Setup a user named ``root`` with a password of ``root``

#### b- Create a database named ``VehiculeDB`` with a table named ``Invoices``:

Use the provided file in SqlHelpers named ``setup.sql`` entering the following command :
```bash
mysql < SqlHelpers/setup.sql -u root -p
```
when prompted for the password enter ``root`` as setup in step **a**

this command will setup your mysql server with a database named VehiculeDB 

and a table named Invoices have examples of invoices.

**OR** 

you can set it up manually using sql commands but *NOTE* that the name 

of the database and the table should match what is noted above.

#### c- Add additional invoices data:

You can use the ``data.sql`` file provided in SqlHelpers and modify it to add additional raws in the database .

After adding elements execute: 

```bash
mysql < SqlHelpers/data.sql -u root -p
```
Same as last step enter ``root`` as your password.

**OR**

you can add individual elements manually and *NOTE* to match the name of the table.


### 4- Build Binaries
you can build the whole project with :
```bash
scripts/build.sh
```
this will generate 4 binaries under the bin folder to use :

1- **ClientTCP** which is the client's binaries
                                                            
2- **ClientServeur** which is the proxy 
                                                            
3- **ServeurTCP** which is the Vehicules serveur

4- **ServeurUDP** which is the parapharmaceutical serveur


**OR**

you can compile the binaries with a compiler of your choice but note that 

the proxy have the following source files: ``ClientServeur.c`` ``ClientServeurUtils.c`` 

## Executing the app

you should execute the 4 provided binaries in a seperate processes and only use the Client

pocesse to use the app where you can:

### 1- Retrieve Parapharmaceutical invoices

### 2- Retrieve Vehicules invoices

### 3- Get Sum of both server's invoices

**NOTE:** To view more details about the project,check the provided pdf file(STILL NOT PROVIDED).
