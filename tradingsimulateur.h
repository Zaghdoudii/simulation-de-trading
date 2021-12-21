#ifndef TRADINGSIMULATEUR_H_INCLUDED
#define TRADINGSIMULATEUR_H_INCLUDED
#include<iostream>
#include <sstream>
#include <string>
#include<vector>
#include<iomanip>
#include<list>
#include<map>
#include<algorithm>
#include <ctime>
#include <cstdlib>

using namespace std;

class Date
{
private:
public:
    int jour;
    int mois;
    int annee;

public:

    void set_jour(int j){jour=j;}
    void set_mois(int m){mois=m;}
    void set_annee(int a){annee=a;}
    void set_date(int j,int m,int a){jour=j;mois=m;annee=a;}
    void Nextjour();
    void Precedentjour();
    Date(int j,int m ,int a): jour(j),mois(m),annee(a){}
    Date(){}
    ~Date(){}
    Date(Date const& d):jour(d.jour),mois(d.mois),annee(d.annee){}
    const int get_jour ()  {return jour;}
    const int get_mois()  {return mois;}
    const int get_annee() {return annee;}
    const string afficher()const {
        stringstream jj,mm,aa;
        string sjour,smois,sannee;
        jj<<jour;jj>>sjour;
        mm<<mois;mm>>smois;
        aa<<annee;aa>>sannee;
        return sjour+'/'+smois+'/'+sannee;}
    bool operator<(const Date& d2) const;
    friend ostream& operator<<(ostream& flux,const Date& date);

};

class Action
{
public:
    string nom_action;
    double prix_action;
    int nbre_action;

public:
    Action(string chaine="action_par_defaut",double p=0.0, int i=0):nom_action(chaine),prix_action(p),nbre_action(i) {}
    double get_prix_action(){return prix_action;}
    const string get_nom_action(){return nom_action;}
    int get_nbre_action(){return nbre_action;}

    void set_nom_action(string chaine){nom_action=chaine;}
    void set_nbre_action(int i){nbre_action=i;}
    void set_prix_action(double d){prix_action=d;}
    friend ostream& operator<<(ostream& flux,const Action& action);

};



template<class T>
class Bourse
{
    protected:
        Date date;
    public:
    Bourse(Date date):date(date){}
    Date& get_date(){return date;}
    void set_date(Date d){date=d;}
    void set_nbreaction(string chaine);
    int get_nbreaction(string chaine);
    virtual T& get_bourseactions()=0;
    virtual map<Date,T >& getHistoriqueActions()=0;
    virtual void afficherHistoriqueAction()=0;
    virtual Action& rechercheAction(string nom)=0;
    virtual bool existeAction(string nom)=0;
    virtual void afficherprixaction()=0;
    void afficherActions();
    Action pos(int i);
};

template<class T>
class BourseMapVL:public Bourse<T>
{
    public:
    T bourseActions;
    map< Date,T > HistoriqueActions;
    BourseMapVL(Date date):Bourse<T>(date){}
    map< Date,T >& getHistoriqueActions(){return HistoriqueActions;}
    T& get_bourseactions(){return bourseActions;}
    void afficherHistoriqueAction();
    Action& rechercheAction(string nom);
    bool existeAction(string nom);
    void afficherprixaction();

};

template<class T,class U>
class Trader
{
protected:
    double montant_inv;
    double montant_res;
    T traderActions;
    string nom;
public:
    Trader(){}
    Trader(string nom,double montant):nom(nom),montant_inv(montant){montant_res=montant_inv;}
    string get_nom(){return nom;}
    const double get_montant_inv(){return montant_inv;}
    double get_montant_res(){return montant_res;}
    bool existeAction(string nom);
    Action& rechercheAction(string nom);
    void set_montant_inv(double solde){ montant_inv=solde;}
    T& get_trader_actions(){return traderActions;}
    virtual void transaction(Bourse<U>* bourse)=0;
    void afficherActions();
    Action& pos(int i);
    void supprimer(string nom);
};
template<class T,class U>
class TraderHumain:public Trader<T,U>
{

public:
    TraderHumain(){}
    TraderHumain(string nom,double montant):Trader<T,U>(nom,montant){}
    void transaction(Bourse<U>* bourse);
private:
    void vendre_action(Bourse<U>* bourse, string nom_action,int nbre_action);
    void acheter_action(Bourse<U>* bourse,string nom_action,int nbre_action);
};

template<class T,class U>
class TraderAlea:public Trader<T,U>
{
public:

    TraderAlea(){}
    TraderAlea(string nom,double montant):Trader<T,U>(nom,montant){}
    void transaction(Bourse<U>* bourse);

};

template<class T,class U>
class TraderAlgo:public Trader<T,U>
{
public:
    TraderAlgo(){}
    TraderAlgo(string nom,double montant):Trader<T,U>(nom,montant){}
    void transaction(Bourse<U>* bourse);
};

template<class T,class U>
class Simulation
{

private:
    int duree;
    int Nbrejour;
    Date dateinitiale;
    Date datefinale;
    Date datecourant;
    string nom_simulation;
    string modeTrader;
    double solde;
    bool gain_perte;
    Trader<T,U>* trader;
    Bourse<U>* bourse;
    map<Date,U> AllActions;

public:
    Simulation(){}
    //~Simulation(){ delete bourse; delete trader;}
    Simulation(Date d1,Trader<T,U>* t,Bourse<U>* b, int i):dateinitiale(d1),trader(t),bourse(b),duree(i)
    {Nbrejour=1;datecourant=dateinitiale;datefinale=dateinitiale;
     for(int i=0;i<duree-1;i++){datefinale.Nextjour();}
    }
    const int get_duree(){return duree;}
    const int get_Nbrejour(){return Nbrejour;}
    void setSolde();
    double  get_solde(){return solde;}
    void set_gainperte(){
        if(solde>trader->get_montant_inv() )
            gain_perte=true;
        else
            gain_perte=false;}
    bool get_gainperte(){return gain_perte;}
    string& getNom(){return nom_simulation;}
    string& getmodetrader(){return modeTrader;}
    Date get_dateinitiale(){return dateinitiale;}
    Date get_datefinale(){return datefinale;}
    Date get_datecourant(){return datecourant;}
    map< Date,U > getAllActions(){return AllActions;}
    void set_datecourante(Date d){datecourant=d;}
    void setAllActions();
    void Next_jour();
    Bourse<U>* get_bourse(){return bourse;}
    Trader<T,U>* get_trader(){return trader;}
    double dernierprixaction(string nom_action);
private:
        void setBourseActions();
};

template<class T,class U>
class Simulations
{
    private:
    public:
    vector<Simulation<T,U> > TousSimulations;

    public:
    Simulations(){}
    ~Simulations(){}

    bool existe(string nom);
    Simulation<T,U> recherche(string nom);

};


#endif // TRADINGSIMULATEUR_H_INCLUDED
