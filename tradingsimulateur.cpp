#include <iostream>
#include <fstream>
#include <sstream>
#include "string.h"
#include "tradingsimulateur.h"
#include<algorithm>

bool operator<=(const Date& d1,const Date& d2)
{
    if (d1.annee!=d2.annee)
        return  d1.annee<d2.annee;
    else if (d1.mois!=d2.mois)
        return d1.mois<d2.mois;
    else
        return d1.jour<=d2.jour;
}
bool Date::operator<(const Date& d2) const
{
    if (annee!=d2.annee)
        return  annee<d2.annee;
    else if (mois!=d2.mois)
        return mois<d2.mois;
    else
        return jour<d2.jour;
}
bool operator!=(const Date& d1,const Date& d2)
{
    return (d1.jour!=d2.jour)||(d1.mois!=d2.mois)||(d1.annee!=d2.annee);
}

ostream& operator<<(ostream& flux,const Date& date)
{
    flux<<date.jour<<"/"<<date.mois<<"/"<<date.annee;
    return flux;
}
ostream& operator<<(ostream& flux,const Action& action)
{
    flux<<setw(10)<<action.nom_action<<"\t\t"<<setw(11)<<action.prix_action<<"\t\t\t\t"<<action.nbre_action;
    return flux;
}

template<class T,class U>
ostream& operator<<(ostream& flux,Simulation<T,U>& simulation)
{
    flux<<"La duree de la Simulation                        : "<<simulation.get_duree()<<" Jours. "<<endl;
    flux<<"La date initiale de la Simulation                : "<<simulation.get_dateinitiale()<<endl;
    flux<<"La date finale de la simulation de la Simulation : "<<simulation.get_datefinale()<<endl;
    flux<<"La date courante de la Simulation                : "<<simulation.get_datecourant()<<endl;
    flux<<"Le nombre de jours passe dans la Simulation      : "<<simulation.get_Nbrejour()<<endl;
    return flux;
}

void Date::Nextjour()
{
    int tab[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if (annee%4==0)
        tab[1]=29;
    if (jour<tab[mois-1])
        jour++;
    else if((jour==tab[mois-1])&&(mois!=12)){jour=1;mois++;}
    else{jour=1;mois=1;annee++;}
}
void Date::Precedentjour()
{
    int tab[12]={31,28,31,30,31,30,31,31,30,31,30,31};
    if (annee%4==0)
        tab[1]=29;
    if (jour!=1)
        jour--;
    else if(jour==1 && mois!=1){jour=tab[mois-2];mois--;}
    else{jour=31;mois=12;annee--;}
}

template<class T,class U>
void Simulation<T,U>::Next_jour()
{
    while(true)
    {
        datecourant.Nextjour();
        Nbrejour++;
        bourse->get_date().Nextjour();
        if(!AllActions[datecourant].empty())
        {
            this->setBourseActions();
            break;
        }
        if(Nbrejour==duree)
            break;
    }
}

template<class T,class U>
bool Trader<T,U>::existeAction(string nom)
{
    for(typename T::iterator it=traderActions.begin();it!=traderActions.end() ;it++)
            if (it->get_nom_action()== nom )
            return true;
    return false;
}
template<class T,class U>
double Simulation<T,U>::dernierprixaction(string nom_action)
{
    double prix=0;
    bool b=false;
    for(typename map<Date,U >::reverse_iterator it=AllActions.rbegin(); it!=AllActions.rend(); it++)
    {
        for(typename U::iterator vt=it->second.begin();vt!=it->second.end();vt++)
        {
            if(vt->get_nom_action()==nom_action)
            {
                prix=vt->get_prix_action();
                b=true;
                break;
            }
        }
        if(b)
                break;
    }
    return prix;
}

template<class T>
Action& BourseMapVL<T>::rechercheAction(string nom)
{
        for(typename T::iterator it=bourseActions.begin();it!=bourseActions.end() ;it++)
            if (it->get_nom_action()== nom )
            return *it;
        Action action("Action_par_defaut",0,0);
        return action;
}

template<class T>
bool BourseMapVL<T>::existeAction(string nom)
{
    for(typename T::iterator it=bourseActions.begin();it!=bourseActions.end() ;it++)
        if (it->get_nom_action()== nom )
            return true;
    return false;
}

template<class T,class U>
Action& Trader<T,U>::rechercheAction(string nom)
{
    for(typename T::iterator it=traderActions.begin();it!=traderActions.end() ;it++)
        if (it->get_nom_action()== nom )
            return *it;
    Action action("Action_par_defaut",0,0);
    return action;
}

template<class T,class U>
void Simulation<T,U>::setAllActions()
{
    string lejour,lemois,lannee,nom,leprix,firstline;
    int jour,mois,annee;
    double prix;
    Action action;
    Date d(dateinitiale);
    ifstream file;
    file.open("prices_simple.csv",ios::in);
    if(!file)
        cerr<<"le fichier n'est pas ouvert ! "<<endl;
    else{
            if(dateinitiale<Date(3,1,2011)){
                file.seekg(0);
            }
            else if (dateinitiale<Date(3,1,2012)){
                file.seekg(2905305);
            }else if (dateinitiale<Date(2,1,2013)){
                file.seekg(5839999);
            }else if (dateinitiale<Date(2,1,2014)){
                file.seekg(8758650);
            }else if (dateinitiale<Date(2,1,2015)){
                file.seekg(11835308);
            }else if (dateinitiale<Date(4,1,2016)){
                file.seekg(14962067);
            }else{
                file.seekg(18128124);
            }
    getline(file,firstline);
    AllActions.clear();
    while(!file.eof())
    {
        stringstream jj,mm,aa,pp;
        getline(file,lejour,'/');
        getline(file,lemois,'/');
        getline(file,lannee,';');
        getline(file,nom,';');
        getline(file,leprix);
        jj<<lejour;jj>>jour;
        mm<<lemois;mm>>mois;
        aa<<lannee;aa>>annee;
        pp<<leprix;pp>>prix;
        d.jour=jour;d.mois=mois;d.annee=annee;
        if( (dateinitiale<=d)&&(d<=datefinale))
            if( (jour==d.get_jour()) && (mois==d.get_mois()) && (annee==d.get_annee()))
            {
                action.set_nom_action(nom);
                action.set_prix_action(prix);
                AllActions[d].push_back(action);

            }
        if(datefinale<d)
            break;
    }
    }
    file.close();
    if(AllActions[datecourant].empty() )
        this->Next_jour();
    else
        this->setBourseActions();
}
template<class T,class U>
void Trader<T,U>::afficherActions()
{
    cout<<"Nom_action\t\tPrix_achat_action\t\tquantite_action"<<endl;
    T vect=this->get_trader_actions();
    for(typename T::iterator it=vect.begin();it!=vect.end();it++)
        cout<<*it<<endl;
}


template<class T,class U>
 void Simulation<T,U>::setBourseActions()
 {
     bourse->get_bourseactions().clear();
     bourse->get_bourseactions()=AllActions[datecourant];
     bourse->getHistoriqueActions()[datecourant]=AllActions[datecourant];
 }
template<class T,class U>
 void TraderHumain<T,U>::vendre_action(Bourse<U>* bourse,string nom_action,int nbre_action)
{
    bool b,a=false;
    Action& action=this->rechercheAction(nom_action);
    if(bourse->existeAction(nom_action))
    {
    double prix=bourse->rechercheAction(nom_action).get_prix_action();
    if(this->existeAction(nom_action))
    {
        if(action.get_nbre_action()>nbre_action)
        {
            this->montant_res+=prix*nbre_action;
            action.nbre_action-=nbre_action;
            b=true;
        }
        else if(action.get_nbre_action()==nbre_action)
        {
            this->montant_res+=prix*action.get_nbre_action();
            this->supprimer(action.get_nom_action());
            a=true;b=true;
        }
        else
            b=false;
    }
    else
        b=false;
    }
    else
        b=false;

    if( b )
            {
                cout<<" vente reussi "<<endl;
                if(a)
                    cout<<" Vous avez vendu tous les actions disponibles. " <<endl;
                system("pause");
            }
            else
            {
                cout<<" vente echoue : Vous ne possede pas "<<nbre_action<< "de l'action ''"<<nom_action<<"'' ! "<<endl;
                system("pause");
            }
            system("cls");
}

template<class T,class U>
void TraderHumain<T,U>::acheter_action(Bourse<U>* bourse,string nom_action,int nbre_action)
{
bool b;
if(bourse->existeAction(nom_action))
{
    double prix=bourse->rechercheAction(nom_action).get_prix_action();
    if(this->existeAction(nom_action))
    {
        if( prix*nbre_action<=this->montant_res )
        {
            Action& act=this->rechercheAction(nom_action);
            this->montant_res-=prix*nbre_action;
            act.nbre_action+=nbre_action;
            b=true;
        }
        else
            b=false;
    }
    else
    {
        if( prix*nbre_action<=this->montant_res )
        {
            Action action(nom_action,prix,nbre_action);
            this->montant_res-=prix*nbre_action;
            this->traderActions.push_back(action);
            b=true;
        }
        else
            b=false;
    }
    if( b )
            {
                cout<<" Achat reussi "<<endl;
                system("pause");
            }
    else
            {
                cout<<" Achat echoue ! "<<endl;
                system("pause");
            }
}
else
    {
        cout<<"L'action ''"<<nom_action<<"'' n'existe pas dans la bourse aujourd'hui !"<<endl;
        system("pause");
    }
    system("cls");
}
template<class T,class U>
void TraderHumain<T,U>::transaction(Bourse<U>* bourse)
{
    while(true)
    {
    cout<<" 1 - Acheter une action        :"<<endl;
    cout<<" 2 - Vendre une action         :"<<endl;
    cout<<" 3 - Return au menu principale :"<<endl;
    int i, nbre; string nom;
    cin>>i;
    switch(i)
    {
    case 1:
        {

            cout<<" Donner le nom d'action a acheter      : ";
            cin>>nom;
            cout<<" Donner la qunatite d'action a acheter :";
            cin>>nbre;
            this->acheter_action(bourse,nom,nbre);
            break;
        }
    case 2:
        {
            cout<<" Donner le nom d'action a vender      : ";
            cin>>nom;
            cout<<" Donner la qunatite d'action a vendre :";
            cin>>nbre;
            this->vendre_action(bourse,nom,nbre);
            break;
        }
    case 3:
        {
            break;
        }
    default:
        {
            cout<<" Choix invalide !"<<endl;
            system("pause");
            system("cls");
            break;
        }
    }
    if (i==3)
        break;
    }
}
template<class T>
void BourseMapVL<T>::afficherHistoriqueAction()
{
    string nom;
    cout<<" Donner le nom d'action : "<<endl;
    cin>>nom;
    cout<<" L'historique de l'action ''"<<nom<<"'' est : "<<endl;
    cout<<"  Date   \t\tPrix_action "<<endl;
    for(typename map< Date,T >::iterator it=getHistoriqueActions().begin();it!=getHistoriqueActions().end();it++)
        {
            T vect=it->second;
            for(typename T::iterator vectit=vect.begin();vectit!=vect.end();vectit++)
            {
                if(vectit->get_nom_action()==nom)
                    cout<<it->first <<"\t\t" <<vectit->get_prix_action()<<endl;
            }

    }
    cout<<endl;
}


template<class T>
void BourseMapVL<T>::afficherprixaction()
{
    string nom;bool b=false;
    cout<<" Donner le nom d'action :"<<endl;;
    cin>>nom;
    Action action;
    for(typename T::iterator it=bourseActions.begin();it!=bourseActions.end();it++)
        if(it->nom_action==nom)
        {
            b=true;
            action=*it;
            break;
        }
    if(b)
    {
        cout<<" Le prix de l'action ''"<<nom<<"'' est "<<action.prix_action<<" ."<<endl;;
    }
    else
        cout<<" L'action ''"<<nom<<"'' n'existe pas !"<<endl;
    cout<<endl;
}

template<class T>
Action Bourse<T>::pos(int i)
{
    int j=0;
    for(typename T::iterator it=this->get_bourseactions().begin();it!=this->get_bourseactions().end();it++)
    {
        if(i==j)
            return *it;
        j++;
    }

}
template<class T,class U>
Action& Trader<T,U>::pos(int i)
{
    int j=0;
    for(typename T::iterator it=this->traderActions.begin();it!=this->traderActions.end();it++)
    {
        if(i==j)
            return *it;
        j++;
    }
    Action action;
    return action;
}
template<class T,class U>
void Trader<T,U>::supprimer(string nom)
{
    T& vect=this->get_trader_actions();
    for(typename T::iterator it=vect.begin();it!=vect.end();it++)
    {
        if(it->nom_action==nom)
        {
            vect.erase(it);
            break;
        }
    }
}


enum{rien,vente,achat};
template<class T,class U>
void TraderAlea<T,U>::transaction(Bourse<U>* bourse)//faire <10 transactions
{
    srand(time(0));
    int nbretran=5+rand()%6,i=0;
    while(true)
    {
        int choix=rand()%3;
        switch(choix)
        {
        case rien:
            {
                break;
            }
        case achat:
            {
                int nbreact=1+rand()%3;
                U bourseActs=bourse->get_bourseactions();
                Action action=bourse->pos(rand()%(bourseActs.size()+1));
                double prix=action.get_prix_action();
                string nom_action=action.get_nom_action();
                if(prix*nbreact<=this->montant_res)
                {
                    this->montant_res-=prix*nbreact;
                    if (this->existeAction(nom_action))
                    {
                        Action& act=this->rechercheAction(nom_action);
                        act.nbre_action+=nbreact;
                    }
                    else
                    {
                        Action act(nom_action,prix,nbreact);
                        this->get_trader_actions().push_back(act);
                    }
                    cout<<" Achat de "<<nbreact<<" l'action ''"<<nom_action<<"'',Le prix d'achat de l'action est "<<prix<<" ."<<endl<<endl;
                    system("pause");
                    break;
                }
                else
                {
                    cout<<"Achat echoue "<<endl<<"Vous n'avez pas la somme d'argent necessaire "<<endl;
                    system("pause");
                    break;
                }
                break;
            }
        case vente:
            {
                if(this->traderActions.empty())
                {
                    cout<<" La Vente echoue ! Le traderAlea n'a pas des actions !"<<endl<<endl;
                    system("pause");
                    break;
                }
                Action& action=this->pos(rand()%(this->traderActions.size()));
                int nbreact=1+rand()%(action.get_nbre_action());
                string nom_action=action.get_nom_action();
                if(bourse->existeAction(nom_action))
                {
                    Action& act=bourse->rechercheAction(nom_action);
                    this->montant_res+=act.prix_action*nbreact;
                    if(nbreact!=action.nbre_action)
                        action.nbre_action-=nbreact;
                    else
                        this->supprimer(nom_action);
                    cout<<" La Vente de "<<nbreact<<" action ''"<<nom_action<<"'',le prix de vente de l'action est : "<<act.prix_action<<" ."<<endl<<endl;
                    system("pause");
                    break;
                }
                else
                {
                    cout<<" La vente de l'action ''"<<nom_action<<"'' est echoue ! L'action n'existe pas dans la bourse aujourd'hui."<<endl<<endl;
                    system("pause");
                    break;
                }
            }
        }
        i++;
        if(i==nbretran || choix==0)
        {
            cout<<"Les transactions d'aujourd'hui sont finis !"<<endl<<endl;
            system("pause");
            break;
        }
    }
}
//Le traderAlgo vend les actions qui ont en gain superieur a 50%
//sinon Le traderAlgo vend les actions qui ont en gain superieur a 20%
//sinon Le traderAlgo vend les actions qui ont en gain superieur a 05%

//Achat la plus chere action qu'on peut acheter

template<class T,class U>
void TraderAlgo<T,U>::transaction(Bourse<U>* bourse)
{
    bool b=false;
    vector<string> vectstring;
    T& traderactions=this->get_trader_actions();
    if(!traderactions.empty()){
    for(typename T::iterator it=traderactions.begin();it!=traderactions.end();it++)
    {
        string nom_action=it->nom_action;
        if(bourse->existeAction(nom_action))
        {
            double prix=bourse->rechercheAction(nom_action).prix_action;
            if(it->prix_action*1.05<=prix)
            {
                this->montant_res+=prix*it->nbre_action;
                vectstring.push_back(nom_action);
                cout<<"Vente de "<<it->nbre_action<<" de l'action ''"<<nom_action<<"'' avec un gain superieur a 05%"<<endl;
                cout<<"   Le prix d'achat d'une action est : "<<it->prix_action<<" ."<<endl;
                cout<<"   Le prix de vente d'une action est : "<<prix<<" ."<<endl;
                b=true;
            }
        }
    }
    if(b==false)
    {
    for(typename T::iterator it=traderactions.begin();it!=traderactions.end();it++)
    {
        string nom_action=it->nom_action;
        if(bourse->existeAction(nom_action))
        {
            double prix=bourse->rechercheAction(nom_action).prix_action;
            if(it->prix_action*1.02<=prix)
            {
                this->montant_res+=prix*it->nbre_action;
                vectstring.push_back(nom_action);
                cout<<"Vente de "<<it->nbre_action<<" de l'action ''"<<nom_action<<"'' avec un gain superieur a 02%"<<endl;
                cout<<"   Le prix d'achat d'une action est : "<<it->prix_action<<" ."<<endl;
                cout<<"   Le prix de vente d'une action est : "<<prix<<" ."<<endl<<endl;
                b=true;
            }
        }
    }
    }
    if(b==false)
    {
    for(typename T::iterator it=traderactions.begin();it!=traderactions.end();it++)
    {
        string nom_action=it->nom_action;
        if(bourse->existeAction(nom_action))
        {
            double prix=bourse->rechercheAction(nom_action).prix_action;
            if(it->prix_action*1.01<=prix)
            {
                this->montant_res+=prix*it->nbre_action;
                vectstring.push_back(nom_action);
                cout<<"Vente de "<<it->nbre_action<<" de l'action ''"<<nom_action<<"'' avec un gain superieur a 01%"<<endl;
                cout<<"   Le prix d'achat d'une action est : "<<it->prix_action<<" ."<<endl;
                cout<<"   Le prix de vente d'une action est : "<<prix<<" ."<<endl<<endl;
                b=true;
            }
        }
    }
    }
    for(vector<string>::iterator it=vectstring.begin(); it!=vectstring.end();it++)
        this->supprimer(*it);
    if(b==false)
    {
        cout<<" PAS DE VENTE ! Toutes les actions possedes ont un gain inferieur strictemenent a 01% !"<<endl<<endl;
    }
    }else{
        cout<<" PAS DE VENTE ! Le Trader n'a pas d'actions a vendre !"<<endl<<endl;
    }

    U bourseactions=bourse->get_bourseactions();
    double prix_max=0;
    string nom_action="Action_par_defaut";
    bool a=false;
    for(typename U::iterator it=bourseactions.begin();it!=bourseactions.end();it++)
    {
        double prix=it->get_prix_action();
        if((prix>prix_max) && (prix<=this->montant_res))
        {
            prix_max=prix;
            nom_action=it->get_nom_action();
            a=true;
        }
    }
    if(this->existeAction(nom_action))
    {
        Action& act=this->rechercheAction(nom_action);
        act.nbre_action++;
        this->montant_res-=prix_max;
    }
    else if(a)
    {
        Action Taction(nom_action,prix_max,1);
        this->montant_res-=prix_max;
        this->get_trader_actions().push_back(Taction);
    }
    if(a)
        cout<<" Achat d'une action ''"<<nom_action<<"'',Le prix d'achat est "<<prix_max<<" ."<<endl<<endl;
    else
        cout<<" Le trader n'a pas l'argent d'acheter une action "<<endl;
}

template<class T>
void Bourse<T>::afficherActions()
{
    cout<<"les Actions disponibles aujourd'hui ("<<this->get_date()<<") :"<<endl<<endl;

    for(typename T::iterator it=this->get_bourseactions().begin();it!=this->get_bourseactions().end();it++)
        {
            cout<<setw(5)<<it->get_nom_action()<<" | ";
        }
}
template<class T,class U>
void Simulation<T,U>::setSolde()
{

    T& actions=trader->get_trader_actions();
    double prix,sol;
    sol=trader->get_montant_res();
    for(typename T::iterator it=actions.begin();it!=actions.end();it++)
    {
        prix=this->dernierprixaction(it->nom_action);
        sol+=prix*it->get_nbre_action();
    }
    solde=sol;
}

template<class T,class U>
bool Simulations<T,U>::existe(string nom)
{
    for(typename vector<Simulation<T,U> >::iterator it=TousSimulations.begin();it!=TousSimulations.end();it++)
    {
        if(it->getNom()==nom)
            return true;
    }
    return false;
}

template<class T,class U>
Simulation<T,U> Simulations<T,U>::recherche(string nom)
{
    for(typename vector<Simulation<T,U> >::iterator it=TousSimulations.begin();it!=TousSimulations.end();it++)
    {
        if(it->getNom()==nom)
            return *it;
    }
    Simulation<T,U> s;
    return s;

}


template class Simulation<vector<Action>,vector<Action> >;
template class Simulation<list<Action>,vector<Action> >;
template class Simulation<list<Action>,list<Action> >;
template class Simulation<vector<Action>,list<Action> >;
template class Simulations<vector<Action>,vector<Action> >;
template class Simulations<list<Action>,vector<Action> >;
template class Simulations<list<Action>,list<Action> >;
template class Simulations<vector<Action>,list<Action> >;
template class Bourse<list<Action> >;
template class Bourse<vector<Action> >;
template class BourseMapVL<list<Action> >;
template class BourseMapVL<vector<Action> >;
template class Trader<vector<Action>,vector<Action> >;
template class Trader<vector<Action>,list<Action> >;
template class Trader<list<Action>,vector<Action> >;
template class Trader<list<Action>,list<Action> >;
template class TraderAlea<vector<Action>,vector<Action> >;
template class TraderAlea<vector<Action>,list<Action> >;
template class TraderAlea<list<Action>,vector<Action> >;
template class TraderAlea<list<Action>,list<Action> >;
template class TraderAlgo<vector<Action>,vector<Action> >;
template class TraderAlgo<vector<Action>,list<Action> >;
template class TraderAlgo<list<Action>,vector<Action> >;
template class TraderAlgo<list<Action>,list<Action> >;
template class TraderHumain<vector<Action>,vector<Action> >;
template class TraderHumain<vector<Action>,list<Action> >;
template class TraderHumain<list<Action>,vector<Action> >;
template class TraderHumain<list<Action>,list<Action> >;
