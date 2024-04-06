#include <iostream>
#include <ctime>
#include <stdlib.h>
#include <string>
#include <unistd.h>//sleep per linux || commentare questa riga se si ha windows
//#include <windows.h> decommentare questa riga se si ha windows
//sleep serviva per mostrare il testo in una funzione

//Grafiche + Gran parte del Gioco (gli Extra) +  alcune funzioni Yifei
//Debugging/Quality check + Gioco + commenti + alcune funzioni Varricchio
using namespace std;

const int sizeR = 5;
const int sizeC = 5;


// Prototipi delle funzioni
void systemClear();
void solitoMenu(string barrier);
void stampaTabellone(int tabelloneDaStampa[][sizeC]);
void aggiornaTabellona(int tabelloneDaStampa[][sizeC], bool tabellone[][sizeC], int x, int y, int &punteggio, int &streak, int &tentativi, int &numeroDiNaviColpite, int &moneteDiOro, int &moneteDiPlatino);
void negozioBoost(int &moneteDiOro, int boost[], int &entrata);
void negozioPozioni(int &moneteDiOro, int &pozioni, int &entrata);
void negozioTalismani(int &moneteDiPlatino, bool talismani[], int &entrata);
void checkFinito(int tentativi, int numeroDiNaviColpite, int &moneteDiOro, int &moneteDiPlatino, string barrier);
void aggiornaLeaderboard(int punteggio, int leaderboard[]);



int main() {

    srand(time(NULL));
    bool gioca, talismani[2] = {false}; // talismani bool perchè si deve sapere se è attivato (true) o disattivato (false)
    int x,y,punteggio,entrata,streak=0,moneteDiOro = 9999,moneteDiPlatino = 9990,leaderboard[5] = {0}; 

  // stampa menù
    string barrier = "{-----------------------------------------}";
    string nave ="    ____\n   |[@]|\n __|[@]|___\n \\        /\n  \\______/"; //decorativa
    cout << barrier << endl << endl << "Benvenuto nella Battaglia Navale!\nGioca per ottenere punteggi sempre più alti.\n\nLeggenda:\n0 = mappa\n7 = nave colpita\n8 = nave mancata\n\n" << barrier << "\n\nNota:\nPer ogni nave affondata sono 200 punti + i tentativi\nrimanenti. Se si colpiscono più navi di \nseguito si avvierà uno stato di streak, cioè un \nbonus di 120 punti per ogni nave affondata.\nLo stato di streak terminerà in seguito ad un colpo mancato.\n\n"<< barrier<< "\n\nCliccare [INVIO] per andare avanti!\n\n" << barrier << endl;
    cin.get();// prende il carattere invio e serve per mandare avanti il programma
    do { //gioco N1

        int tentativi = 10, numeroDiNaviColpite = 0, boost[2] = {0}, pozioni = 0; // boost[0] tentativi | boost[1] monete (vengono resettati a ogni ciclo)
        bool tabellone[sizeR][sizeC] = {{0}};
        int tabelloneDaStampa[sizeR][sizeC] = {{0}};
        punteggio = 0;
        do{ // utilizzato per ritornare alla schermata principale dai negozi N2
            systemClear();
            cout << barrier << endl << endl << "Premere\n\n[0] per vedere il leaderboard\n[1] per entrare nel negozio\n[2] per uscire dal gioco(Dati non salvati)\n[3] per iniziare una nuova partita\n\n" << barrier << endl;

            cin >> entrata;

            switch(entrata){ //switch per i vari input di entrata
               // Leaderboard (funziona)
              case 0: 
                  systemClear();
                  cout << barrier <<"\n\nLeaderboard Personale: \n\n";
                  for (int i = 0; i < 5; i++){
                      cout << "[" << i+1 << "]: " << leaderboard[i] << endl;
                  }
                  solitoMenu(barrier);
                  cin >> entrata;

                  if (entrata == 0){ //gioca
                      systemClear();
                    //esci
                  } else if (entrata == 1){
                      return 0;
                  }

                  break;
              case 1: // Negozio (funziona)
                  do { //N3
                    systemClear();
                    cout << barrier << endl << endl << "Benvenuto nel Negozio\nIn questo negozio puoi comprare boost, pozioni e accessori.\n\nAttualmente possiedi:\nMonete D'oro: " << moneteDiOro << "\nMonete Di Platino: " << moneteDiPlatino << endl << endl << "In che sezione vuoi andare\n\n[1] Boost\n[2] Pozioni\n[3] Talismani\n\n"<< barrier << endl << endl;

                    do { 
                        cin >> entrata;
                      if (cin.fail()){
                      cin.clear();
                        cin.ignore(1000,'\n');
                       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
                        return 0;
                      }
                        if (entrata < 1 || entrata > 3) {
                            cout << "Inserisci un numero valido!!\n";
                        }
                    } while(entrata > 3 || entrata < 1);
                    if (entrata == 1){ // Boost (funziona)
                        systemClear();
                        cout << barrier << "\n\nBoost\n\nTentativi\n[1] Boost Tentativi (+2 tentativi) Costo: 20 monete d'oro\n[2] Boost Tentativi (+4 tentativi) Costo: 35 monete d'oro\n[3] Boost Tentativi (+6 tentativi) Costo: 50 monete d'oro\n\nMonete\n[4] Boost Monete D'oro (+5 monete) Costo: 25 monete d'oro\n[5] Boost Monete D'oro (+10 monete) Costo: 40 monete d'oro\n[6] Boost Monete D'oro (+20 monete) Costo: 70 monete d'oro\n\n[8] per uscire dal negozio\n\n" << barrier << endl;

                        negozioBoost(moneteDiOro,boost,entrata);
                        solitoMenu(barrier);
                        cin >> entrata;
                      if (cin.fail()){
                      cin.clear();
                        cin.ignore(1000,'\n');
                       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
                        return 0;
                      }
                    } else if (entrata == 2){ // Pozioni (funziona)
                        systemClear();
                        cout << barrier << "\n\nPozioni\n\nVisione\n[1] Pozione Visione (+2 di visione) Costo: 25 monete d'oro\n[2] Pozione Visione (+3 di visione) Costo: 40 monete d'oro\n[3] Pozione Visione (+5 di visione) Costo: 70 monete d'oro\n\n[8] per uscire dal negozio\n\n" << barrier << endl;
                        negozioPozioni(moneteDiOro,pozioni,entrata);
                        solitoMenu(barrier);
                        cin >> entrata;
                      if (cin.fail()){
                      cin.clear();
                        cin.ignore(1000,'\n');
                       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
                        return 0;
                      }
                    } else if (entrata == 3){  //Accessori (funziona)
                        systemClear();
                        cout << barrier << "\n\nTalismani\n\nTentativi\n[1] Talismano Tentativi (+5 tentativi Permanent) Costo: 200 monete di platino\n\nMonete\n[2] Talismano Monete D'oro (+40 monete Permanent) Costo: 200 monete di platino\n\n" << barrier << endl;;
                        negozioTalismani(moneteDiOro,talismani,entrata);
                        solitoMenu(barrier);
                        cin >> entrata; 
                      if (cin.fail()){
                      cin.clear();
                        cin.ignore(1000,'\n');
                       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
                        return 0;
                      }
                    }

                  } while (entrata == 7); // utilizzato per ritornare alla sezione di select del negozio delle pozioni N3
                  break;
              case 2: // Exit
                  return 0;
              case 3:
                  systemClear();
                  break;
            }
        } while (entrata == 8); // utilizzato per ritornare alla schermata principale  N2

//genera le caselle per le navi (funziona)
        for (int i = 0; i < 5; i++){
            int r = rand()%sizeR, c = rand()%sizeC;
            //se la casella già occupata è 0 diventa 1 se no ri fai il ciclo (per navi)
            if (tabellone[r][c] == 0) {
                tabellone[r][c] = 1;
            } else {
                i--;
            }
        }
//diamo il boost per i tentativi se le condizioni sono vere (funziona)
        tentativi += boost[0];
        if (talismani[0]){
            tentativi +=  5;
        }
        string posizioniBarche[pozioni]; // rivelazione caselle (Effetto pozione)
        for (int i = 0; i < pozioni; i++) { //(funziona)
            int r = rand()%sizeR, c = rand()%sizeC;
            // rivela se in quella casella c'è una nave o no
            if (tabellone[r][c] == 1) {
                posizioniBarche[i] = "c'è la presenza di una nave alle cordinate ( " + to_string(r) + " ; " + to_string(c) +" )";
            } else {
                posizioniBarche[i] = "non ci sono navi alle cordinate ( "+ to_string(r) + " ; " + to_string(c) +" )";
            }
        }
      while(tentativi>0){ //N4

            systemClear();
            cout << nave << endl << endl;
            for(int i = 0; i < pozioni; i++){
                cout << posizioniBarche[i] << endl;
            } //Ti dice le posizioni e poi matrice

            cout << endl;
            stampaTabellone(tabelloneDaStampa);
            cout <<"\nTentativi rimanenti: " << tentativi << endl << endl;
            cout << barrier <<"\n\nInserisci le 2 coordinate ( righe , colonne , X e Y se è più chiaro)\n\n"<< barrier << endl;
        do { //N5
                cin >> x >> y;
          if (cin.fail()){
          cin.clear();
            cin.ignore(1000,'\n');
           cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
            return 0;
          }
              //se hai già colpito la coordinata
                if (tabelloneDaStampa[x][y]==7 ||tabelloneDaStampa[x][y]==8){
                    cout<<"Hai già colpito queste coordinate riprova\n";


                } else if (x<0 || x>sizeR-1 || y<0 || y>sizeC-1){ //se superi le coordinate
                    cout<<"Coordinate non valide riprova\n";


                }
            } while(tabelloneDaStampa[x][y]==7 ||tabelloneDaStampa[x][y]==8 || x<0 || x>sizeR-1 || y<0 || y>sizeC-1); //N5
          aggiornaTabellona(tabelloneDaStampa,tabellone,x,y,punteggio,streak,tentativi,numeroDiNaviColpite,moneteDiOro, moneteDiPlatino); //funzione per aggiornare tabellone e il resto
        sleep(1);
      }  //N4 while
      checkFinito(tentativi, numeroDiNaviColpite, moneteDiOro, moneteDiPlatino,barrier);
                if (tentativi==0 ||numeroDiNaviColpite==5){
                  if (talismani[1]){
                      moneteDiOro += 40;
                  } 
                aggiornaLeaderboard( punteggio, leaderboard);
                moneteDiOro += boost[1];
                  moneteDiOro += (numeroDiNaviColpite*2);
                cout<<"\nInserisci 1 se vuoi rigiocare se no premi qualsiasi altro numero: ";

                cin>>gioca;
                  if (cin.fail()){
                  cin.clear();
                    cin.ignore(1000,'\n');
                   cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
                    return 0;
                  }
                  if (gioca!=1){
                    return 0;
                  }
                }
                systemClear();


    } while(gioca==1); //fine gioco //N1
    return 0;
}

//pulisce la console attraverso una speciale combinazione di caratteri (solo Linux)
void systemClear(){
    cout << "\033[2J\033[1;1H"; //commentare se si ha windows
  
}
//solito menù 
void solitoMenu(string barrier){
   cout << endl << barrier<< "\n\nPremere\n\n[0] per giocare\n[1] per uscire dal gioco(Dati non salvati)\n[8] Home \n\n" << barrier << endl;
}
//stampa il tabellone da stampa (Yifei)
void stampaTabellone(int tabelloneDaStampa[][sizeC]){
    cout << "    ";
    for (int i = 0; i < 5; i++){ //prima riga
        cout << i << "   ";
    }
  //seconda riga
    cout << endl << "   " << "__________________" << endl;
    for (int i = 0; i < 5; i++){ 
        cout << i << " |"; //coordinata + barra
        for (int j = 0; j < 5; j++){
            cout <<"["<< tabelloneDaStampa[i][j] << "] "; //matrice
        }
        cout << endl;
    }
}
//Aggiorna i numeri nel tabellone (Insieme)
void aggiornaTabellona(int tabelloneDaStampa[][sizeC],bool tabellone[][sizeC],int x,int y,int &punteggio,int &streak,int &tentativi, int &numeroDiNaviColpite, int &moneteDiOro, int &moneteDiPlatino){ //numeroDiNaviColpite utilizzato per il negozio
  //se hai preso una nave stampa 7
    if (tabellone[x][y]==1){
        tabelloneDaStampa[x][y]=7;
        punteggio+=200+tentativi;
        numeroDiNaviColpite++;
      //se hai colpito più navi di fila
        if (streak>=1){
            punteggio+=120;
           if (streak==5){
             cout<<"\n WOW HAI COLPITO 5 NAVI DI FILA!! Ti sei meritato delle monete di platino e d'oro";
             moneteDiPlatino=moneteDiPlatino+10;
             moneteDiOro=moneteDiOro+20;
           }
        }
        cout<<"Nave colpita, ottimo lavoro soldato!\nAspetta 1 secondo prima di ricolpire\n";
        streak = streak+1;
    }else{
      //se hai mancato la nave stampa 8
        tabelloneDaStampa[x][y]=8;
        cout<<"Nave mancata riprova soldato!\nAspetta 1 secondo prima di ricolpire\n";
        streak = false;
        tentativi--;
    }
}
//Diminuisce i tentativi se hai mancato (Varricchio)

void negozioBoost(int &moneteDiOro, int boost[], int &entrata) {
    do {
        cin >> entrata;
      if (cin.fail()){
      cin.clear();
        cin.ignore(1000,'\n');
       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
        exit(69);
      }
        switch (entrata) {
            case 1:
                if (moneteDiOro >= 20) {
                    boost[0] += 2;
                    moneteDiOro -= 20;
                    cout << "Hai comprato un boost tentativi (+2)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 2:
                if (moneteDiOro >= 35) {
                    boost[0] += 4;
                    moneteDiOro -= 35;
                    cout << "Hai comprato un boost tentativi (+4)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 3:
                if (moneteDiOro >= 50) {
                    boost[0] += 6;
                    moneteDiOro -= 50;
                    cout << "Hai comprato un boost tentativi (+6)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 4:
                if (moneteDiOro >= 25) {
                    boost[1] += 5;
                    moneteDiOro -= 25;
                    cout << "Hai comprato un boost monete (+5)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 5:
                if (moneteDiOro >= 40) {
                    boost[1] += 10;
                    moneteDiOro -= 40;
                    cout << "Hai comprato un boost monete (+10)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 6:
                if (moneteDiOro >= 70) {
                    boost[1] += 20;
                    moneteDiOro -= 70;
                    cout << "Hai comprato un boost monete (+20)!" << endl;
                } else {
                    cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
                }
                break;
            case 7:
                return; // Uscita dalla funzione
            case 8:
                systemClear(); 
                break;
            default:
                cout << "Valore non valido, riprova\n";
                break;
        }
    } while (entrata <= 0 || entrata > 8);
}

void negozioPozioni(int &moneteDiOro, int &pozioni, int &entrata) {
    do {
        cin >> entrata;
      if (cin.fail()){
      cin.clear();
        cin.ignore(1000,'\n');
       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
        exit(69);
      }
        switch (entrata) {
            case 1:
              if (moneteDiOro < 25) {

                  cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";

              } else {
                pozioni += 2;
                moneteDiOro -= 25;
                cout << "Hai comprato una pozione di visione (+2)!" << endl;

              }
          break;
            case 2:
              if (moneteDiOro<40) {
                cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";

              } else {
                pozioni += 3;
                moneteDiOro -= 40;
                cout << "Hai comprato una pozione di visione (+3)!" << endl;
              }
                break;
            case 3:
              if (moneteDiOro < 25) {
                  cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
              } else {
                pozioni += 5;
                moneteDiOro -= 70;
                cout << "Hai comprato una pozione di visione (+5)!" << endl;
              }
                break;
            case 7:
                return; // Uscita dalla funzione
            case 8:
                systemClear(); 
            default:
                cout << "Valore non valido, riprova\n";
                break;
        }
    } while (entrata <= 0 || entrata > 8);
}

void negozioTalismani(int &moneteDiPlatino, bool talismani[], int &entrata) {
    do {
        cin >> entrata;
      if (cin.fail()){
      cin.clear();
        cin.ignore(1000,'\n');
       cout<<"Hai molto probabilmente inserito un carattere, non ci fosse questa scritta imploderebbe il programma, purtroppo devi riavviarlo";
        exit(69) ;
      }
      if (moneteDiPlatino<200){
        entrata=7;
        cout << "Non hai abbastanza monete!!\ngioca per guadagnarle!!\n\n";
      }
        switch (entrata) {
            case 1:
              if (talismani[0]){
                cout<<"Hai già comprato questo oggetto!\n";
              } else {
                    talismani[0] = true;
                    moneteDiPlatino -= 200;
                    cout << "Hai comprato un Talismano di Tentativi (+5)!" << endl;
              }
                break;
            case 2:
              if (talismani[1]){
                cout<<"Hai già comprato questo oggetto!\n";
              } else {
                    talismani[1] = true;
                    moneteDiPlatino -= 200;
                    cout << "Hai comprato un Talismano di Monete d'oro (+40)!" << endl;               }
                break;
            case 7:
                return; // Uscita dalla funzione
            case 8:
                systemClear(); 
                break;
            default:
                cout << "Valore non valido, riprova\n";
                break;
        }
    } while (entrata <= 0 || entrata > 8);
}

void checkFinito(int tentativi, int numeroDiNaviColpite, int &moneteDiOro, int &moneteDiPlatino,string barrier) {
  if (tentativi==0){// INIZIO IF FINE GIOCO
    cout<< endl << barrier << "\n\nHai finito i tentativi, vuoi giocare di nuovo?\n\n" << barrier << endl<<endl;
  }else if (numeroDiNaviColpite==5) {
    cout<< endl << barrier << "\n\nWow sei riuscito a beccarle tutte e 5! Complimenti soldato ti sei meritato una promozione!\n\n\n" << barrier << endl<<endl;
    cout<< endl << barrier << "\n\nHai finito i tentativi, vuoi giocare di nuovo?\n\n" << barrier << endl<<endl;
    moneteDiOro+=5;
    moneteDiPlatino+=1;
  }
}
void aggiornaLeaderboard(int punteggio, int leaderboard[]) { 
    int tempPos = -1; // Verifica se il punteggio è maggiore dei punteggi nella leaderboard 
    for (int i = 0; i < 5; i++) { 
        if (punteggio > leaderboard[i]) { 
            tempPos = i; // Salva la posizione dove il punteggio deve essere inserito 
            break; 
        } 
    } // Se il punteggio è maggiore di almeno uno dei punteggi nella leaderboard 
    if (tempPos != -1) { // Sposta i punteggi successivi di una posizione in avanti per fare spazio al nuovo punteggio 
        for (int i = 4; i > tempPos; i--) { 
            leaderboard[i] = leaderboard[i - 1]; 

        } // Inserisci il nuovo punteggio nella posizione corretta 
        leaderboard[tempPos] = punteggio; 
    }
} 

