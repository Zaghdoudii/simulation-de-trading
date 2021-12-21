#include <iostream>
#include <fstream>
#include <sstream>
#include "string.h"
#include "tradingsimulateur.h"


using namespace std;


int main()
{
    Simulations<vector<Action>,list<Action> > tousSimulations;
    while(true)
    {
        int i;
        system("cls");
        cout<<" \t\t\t -----APPLICTION DE TRADING SIMULATION----- "<<endl;
        cout<<" 1 - Creer une simultaion              : "<<endl;
        cout<<" 2 - Consulter les simulations passes  : "<<endl;
        cout<<" 0 - Exit                              : "<<endl;
        cin>>i;
        system("cls");
        switch(i)
        {
        case 1:
        {
        int indice,choix,duree,jour,mois,annee;string nom_s;
        cout<<"NOUS ALLONS FAIRE UNE SIMULATION DE TRADING. IL FAUT DONNER CES INFORMATIONS :"<<endl;
        while(true){
        cout<<" Donner un nom pour la simulation :"<<endl;
        cin>>nom_s;
        if(tousSimulations.existe(nom_s))
        {
            system("cls");
            cout<<" Il faut changer le nom de simulation, Ce nom existe deja !"<<endl;
        }
        else
            break;
        }
        cout<<"DONNEZ LA DUREE EN JOUR DE LA SIMULATION : ";
        cin >>duree;
        cout<<"DONNEZ LA DATE INITIALE DE LA SIMULATION : "<<endl;
        while(true){
        cout<<" LA DATE INITIALE DOIT ETRE ENTRE 04/01/2010 ET 30/12/2016 : "<<endl;
        cout<<"DONNEZ LE JOUR DE LA DATE : ";
        cin >>jour;
        cout<<"DONNEZ LE MOIS DE LA DATE : ";
        cin >>mois;
        cout<<"DONNEZ L'ANNEE DE LA DATE : ";
        cin >>annee;
        system("cls");
        if(! ( (Date(jour,mois,annee)<Date(4,1,2010)) || (Date(30,12,2016)<Date(jour,mois,annee) ) ) )
            break;
        else
            cout<<"REDONNER LA DATE INTIALE DE LA SIMULATION :"<<endl;
        }
        bool fin=false;

        while(true){
        cout<<" VOUS ALLEZ CHOISIR LE TYPE DE TRADEUR :"<<endl<<"   0 - TRADEUR HUMAIN :"<<endl;
        cout<<"   1 - TRADEUR ALGORITHMIQUE RANDOM :"<<endl<<"   2 - TRADEUR ALGORITHMIQUE SEMI-INTELLIGENT :"<<endl;
        cin>>indice;
        switch(indice)
        {
        case 0:
        {
            double montant;string nom;
            cout<<" Donner le nom de Trader :";
            cin>>nom;
            cout<<" Donner le montant a investir :";
            cin>>montant;
            Simulation<vector<Action> ,list<Action> > simulation(Date(jour,mois,annee),new TraderHumain<vector<Action>,list<Action> >(nom,montant),new BourseMapVL<list<Action> >(Date(jour,mois,annee)),duree);
            simulation.getNom()=nom_s;
            simulation.getmodetrader()="TraderHumain";
            simulation.setAllActions();
            system("cls");
            bool terminer=false;
            while(true)
            {
            cout<<"    GESTION D'UNE SIMULATION DE TRADING"<<endl<<endl;
            cout<<" 1 - Faire des transactions                       :"<<endl;
            cout<<" 2 - Afficher les informations sur le Trader      :"<<endl;
            cout<<" 3 - Afficher les actions disponibles aujourd'hui :"<<endl;
            cout<<" 4 - Afficher l'historique d'une action           :"<<endl;
            cout<<" 5 - Afficher les informations sur la Simulation  :"<<endl;
            cout<<" 6 - passer au jour suivant                       :"<<endl;

            cin>>choix;
            system("cls");
            switch(choix)
            {
            case 1:
                {
                    simulation.get_trader()->transaction(simulation.get_bourse());
                    system("cls");
                    break;
                }
            case 2:
                {
                    cout<<" Le montant d'investissement  : "<<simulation.get_trader()->get_montant_inv()<<endl;
                    cout<<" Le montant restant           : "<<simulation.get_trader()->get_montant_res()<<endl;
                    if(simulation.get_trader()->get_trader_actions().size()==0)
                        cout<<" Le Trader n'a pas d'actions !";
                    else
                    {
                        cout<<" Les actions possede sont     : "<<endl;
                        simulation.get_trader()->afficherActions();
                    }
                    cout<<endl<<endl;
                    system("pause");
                    system("cls");
                    break;
                }
            case 3:
                {

                    int choix;
                    while(true)
                    {
                    simulation.get_bourse()->afficherActions();
                    cout<<endl<<" 1 - consulter le prix d'un action :"<<endl;
                    cout<<" 2 - returner au menu principale   :"<<endl;

                    cin>>choix;
                    switch(choix)
                    {
                    case 1 :
                        {
                            simulation.get_bourse()->afficherprixaction();
                            system("pause");
                            break;
                        }
                    case 2 :
                        {
                            break;
                        }
                    default:
                        {
                            cout<<"choix invalide !"<<endl;
                            system("pause");
                            system("cls");
                            break;
                        }
                    }
                    if(choix==2)
                        break;
                    }
                    system("cls");
                    break;
                }
            case 4:
                {
                    simulation.get_bourse()->afficherHistoriqueAction();
                    system("pause");
                    system("cls");
                    break;
                }
            case 5:
                {
                    cout<<"La duree de la Simulation                        : "<<simulation.get_duree()<<" Jours. "<<endl;
                    cout<<"La date initiale de la Simulation                : "<<simulation.get_dateinitiale()<<endl;
                    cout<<"La date finale de la simulation de la Simulation : "<<simulation.get_datefinale()<<endl;
                    cout<<"La date courante de la Simulation                : "<<simulation.get_datecourant()<<endl;
                    cout<<"Le nombre de jours passe dans la Simulation      : "<<simulation.get_Nbrejour()<<endl<<endl;
                    system("pause");
                    system("cls");
                    break;
                }
            case 6:
                {
                    if(fin)
                    {
                        cout<<" La simulation est termine ."<<endl;
                        terminer=true;
                        system("pause");
                        system("cls");
                        break;
                    }
                    simulation.Next_jour();
                    if(simulation.get_duree()==simulation.get_Nbrejour())
                        fin=true;
                    cout<<" Nous avons passe au jour suivant, C'est le "<<simulation.get_datecourant()<<" ."<<endl<<endl;
                    system("pause");
                    system("cls");
                    break;
                }
            default:
                {
                    cout<<" Choix invalid !!"<<endl;
                    system("pause");
                    system("cls");
                    break;
                }
            }
            if(terminer)
            {
                break;
            }
        }
        tousSimulations.TousSimulations.push_back(simulation);
        break;
        }
        case 1:
            {
                double montant;string nom;
                cout<<" Donner un nom au TraderAlea :";
                cin>>nom;
                cout<<" Donner le montant a investir :";
                cin>>montant;
                system("cls");
                Simulation<vector<Action>,list<Action> > simulation(Date(jour,mois,annee),new TraderAlea<vector<Action>,list<Action> >(nom,montant),new BourseMapVL<list<Action> >(Date(jour,mois,annee)),duree);
                simulation.getNom()=nom_s;
                simulation.getmodetrader()="TraderAleatoire";
                simulation.setAllActions();
                while(true)
                {
                    cout<<"\t\t\t ----INFORMATIONS SUR LA SIMULATION----"<<endl;
                    cout<<"La duree de la Simulation                        : "<<simulation.get_duree()<<" Jours. "<<endl;
                    cout<<"La date initiale de la Simulation                : "<<simulation.get_dateinitiale()<<endl;
                    cout<<"La date finale de la simulation de la Simulation : "<<simulation.get_datefinale()<<endl;
                    cout<<"La date courante de la Simulation                : "<<simulation.get_datecourant()<<endl;
                    cout<<"Le nombre de jours passe dans la Simulation      : "<<simulation.get_Nbrejour()<<endl<<endl;
                    cout<<"\t\t\t ----INFORMATIONS SUR LE TRADER----"<<endl;
                    cout<<" Le montant d'investissement  : "<<simulation.get_trader()->get_montant_inv()<<endl;
                    cout<<" Le montant restant           : "<<simulation.get_trader()->get_montant_res()<<endl;
                    if(simulation.get_trader()->get_trader_actions().size()==0)
                        cout<<" Le Trader n'a pas d'actions !"<<endl<<endl;
                    else
                    {
                        cout<<" Les actions possede sont     : "<<endl;
                        simulation.get_trader()->afficherActions();
                        cout<<endl;
                    }
                    if(fin)
                    {
                        cout<<"La simulation est termine !"<<endl<<endl;

                        break;
                    }else{
                    cout<<" Les transactions va commencer maintenant :"<<endl;
                    system("pause");
                    system("cls");
                    simulation.get_trader()->transaction(simulation.get_bourse());
                    if(simulation.get_duree()==simulation.get_Nbrejour())
                        fin=true;
                    else
                        simulation.Next_jour();
                    system("cls");
                    }
                }
                tousSimulations.TousSimulations.push_back(simulation);
                system("pause");
                break;
            }
        case 2:
            {
                double montant; string nom;
            cout<<" Donner un nom au TraderAlgoritmique :";
            cin>>nom;
            cout<<" Donner le montant a investir :";
            cin>>montant;
            system("cls");
            Simulation<vector<Action>,list<Action> > simulation(Date(jour,mois,annee),new TraderAlgo<vector<Action>,list<Action> >(nom,montant),new BourseMapVL<list<Action> >(Date(jour,mois,annee)),duree);
            simulation.getNom()=nom_s;
            simulation.getmodetrader()="TraderAlgorithmique";
            simulation.setAllActions();
            while(true)
                {
                    cout<<"\t\t\t ----INFORMATIONS SUR LA SIMULATION----"<<endl;
                    cout<<"La duree de la Simulation                        : "<<simulation.get_duree()<<" Jours. "<<endl;
                    cout<<"La date initiale de la Simulation                : "<<simulation.get_dateinitiale()<<endl;
                    cout<<"La date finale de la simulation de la Simulation : "<<simulation.get_datefinale()<<endl;
                    cout<<"La date courante de la Simulation                : "<<simulation.get_datecourant()<<endl;
                    cout<<"Le nombre de jours passe dans la Simulation      : "<<simulation.get_Nbrejour()<<endl<<endl;
                    cout<<"\t\t\t ----INFORMATIONS SUR LE TRADER----"<<endl;
                    cout<<" Le montant d'investissement  : "<<simulation.get_trader()->get_montant_inv()<<endl;
                    cout<<" Le montant restant           : "<<simulation.get_trader()->get_montant_res()<<endl;
                    if(simulation.get_trader()->get_trader_actions().size()==0)
                        cout<<" Le Trader n'a pas d'actions !"<<endl<<endl;
                    else
                    {
                        cout<<" Les actions possede sont     : "<<endl;
                        simulation.get_trader()->afficherActions();
                        cout<<endl;
                    }
                    if(fin)
                    {
                        cout<<"La simulation est termine !"<<endl<<endl;
                        system("pause");
                        system("cls");

                        break;
                    }
                    else{
                    cout<<" Les transactions va commencer maintenant :"<<endl;
                    cout<<endl;
                    system("pause");
                    system("cls");
                    simulation.get_trader()->transaction(simulation.get_bourse());
                    system("pause");
                    if(simulation.get_duree()==simulation.get_Nbrejour())
                        fin=true;
                    else
                        simulation.Next_jour();
                    system("cls");
                    }
                }
                tousSimulations.TousSimulations.push_back(simulation);
            break;
            }
        default:
            {
            cout<<" Choix invalide !"<<endl<<endl;
            system("pause");
            system("cls");
            break;
            }
        }

        if(indice==1 ||indice==0 ||indice==2)
            break;
        }
        break;
        }
        case 2:
            {
                string nom;
                if(tousSimulations.TousSimulations.size()!=0)
                {
                    cout<<" Il existe "<<tousSimulations.TousSimulations.size()<<" simulations :"<<endl;
                    cout<<endl<<" Pour avoir les information d'une simulation, Donner le nom de la simulation : "<<endl;
                    cin>>nom;
                    if(tousSimulations.existe(nom))
                    {
                        Simulation<vector<Action>,list<Action> > s=tousSimulations.recherche(nom);
                        cout<<"\t\t\t ----INFORMATIONS SUR LA SIMULATION----"<<endl;
                        cout<<"Le nom de la Simulation                          : "<<s.getNom()<<endl;
                        cout<<"La duree de la Simulation                        : "<<s.get_duree()<<" Jours. "<<endl;
                        cout<<"La date initiale de la Simulation                : "<<s.get_dateinitiale()<<endl;
                        cout<<"La date finale de la simulation de la Simulation : "<<s.get_datefinale()<<endl;
                        cout<<"La date courante de la Simulation                : "<<s.get_datecourant()<<endl;
                        cout<<"Le nombre de jours passe dans la Simulation      : "<<s.get_Nbrejour()<<endl;
                        cout<<"Le mode de Trader de la Simulation               : "<<s.getmodetrader()<<endl;
                        cout<<"\t\t\t ----INFORMATIONS SUR LE TRADER----"<<endl;
                        cout<<" Le nom de trader est         : "<<s.get_trader()->get_nom()<<endl;
                        cout<<" Le montant d'investissement  : "<<s.get_trader()->get_montant_inv()<<endl;
                        cout<<" Le montant restant           : "<<s.get_trader()->get_montant_res()<<endl;
                        s.setSolde();
                        s.set_gainperte();
                        double sol=s.get_solde();
                        cout<<" Le Solde                     : "<<sol<<endl;
                        double inv=s.get_trader()->get_montant_inv();
                        if(s.get_gainperte())
                        {
                            cout<<" Le resultat de trading       : GAIN "<<endl;
                            cout<<" Le poucentage de gain        : "<<((sol-inv)/inv)*100<<"% "<<endl;
                        }
                        else
                        {
                            cout<<" Le resultat de trading       : PERTE "<<endl;
                            cout<<" Le poucentage de perte       : "<<((inv-sol)/inv)*100<<"% "<<endl;
                        }
                    }
                    else
                    {
                        cout<<" Cette simulation n'existe pas !"<<endl;
                    }
                }
                else
                {
                    cout<<" Il n'exite pas de Simulations !"<<endl;

                }
                system("pause");
                system("cls");
                break;
            }
        case 0 :
            {
                exit(0);
            }
        default:
            {
                cout<<" Choix invalide !"<<endl;
                system("pause");
                system("cls");
                break;
            }
            break;
        }
    }
    return 0;
}
