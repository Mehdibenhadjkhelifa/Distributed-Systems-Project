if [ ! -d "./bin" ]
then
    mkdir bin
fi
gcc -o bin/ClientTCP src/ClientTCP.c 
gcc -o bin/ClientServeur src/ClientServeur.c src/ClientServeurUtils.c
gcc -o bin/ServeurUDP src/ServeurUDP.c 
if [ $# -gt 0 ] && [ "$1" == "nixos" ]; then
gcc -o bin/ServeurTCP src/ServeurTCP.c -L/nix/store/mifgaqzlghrdy273rdfwlkphyfwcc4nh-mysql-8.0.34/lib -lmysqlclient -I/nix/store/mifgaqzlghrdy273rdfwlkphyfwcc4nh-mysql-8.0.34/include 
else
gcc -o bin/ServeurTCP src/ServeurTCP.c -lmysqlclient 
fi

