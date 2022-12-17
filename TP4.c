#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma region UL
static int IDENTIFIANT_UL= 260;
static int NOMBRE_UL =261;
static int OPERATEUR_ARETHMETIQUE= 262;
static int SEPARATEUR_VIRGULE =263;
static int SEPARATEUR_POINT_VIRGULE= 264;
static int PARENTHESE_OUVRANTE =265;
static int PARENTHESE_FERMANTE= 266;
static int OPERATEUR_RELATIONNEL= 267;
static int OPERATEUR_LOGIQUE =268;
static int SELECT_KW =269;
static int FROM_KW =270;
static int WHERE_KW =271;
static int COUNT_KW =272;
static int DISTINCT_KW =273;
static int EPSILON =111;
static int FIN_SUITE_UL  =333;
static int ERREUR =-99;
#pragma endregion

typedef struct Unite_Lexicale {
    char Lexeme[20];
    int Code;
    struct Unite_Lexicale* Suivant;
}UL;

UL* Ajouter_Fin(UL* SuiteUL,char *lex){
    UL* nvsuite = malloc(sizeof(UL));
    strcpy(nvsuite->Lexeme,lex);
    nvsuite->Suivant = NULL;

    if(SuiteUL == NULL){
        return nvsuite;
    }
    UL* temp = SuiteUL;
    while (temp->Suivant != NULL)
    {
        temp = temp->Suivant;
    }
    temp->Suivant = nvsuite;
    return SuiteUL;
}

void Afficher(UL* SuiteUL){
    UL* temp = SuiteUL;
    printf("Affichage de la suite UL\n");
    while (temp != NULL)
    {
        printf("LEXEME = [%s]\t\t\tCode = [%d]\n",temp->Lexeme,temp->Code);
        temp = temp->Suivant;
    }   
}


int Detecter_Sep(char* mot){
    if(!strcmp(mot,";")){
        return SEPARATEUR_POINT_VIRGULE;
    }
    if(!strcmp(mot,",")){
        return SEPARATEUR_VIRGULE;
    }
    return 0;
}
int Detecter_OP(char* mot){
    if(!strcmp(mot,"+") ||!strcmp(mot,"-") ||!strcmp(mot,"*") || !strcmp(mot,"/")){
        return OPERATEUR_ARETHMETIQUE;
    }
    return 0;
}
int Detecter_Parenthese(char* mot){
    if(!strcmp(mot,"(")){
        return PARENTHESE_OUVRANTE;
    }
    if(!strcmp(mot,")")){
        return PARENTHESE_FERMANTE;
    }
    return 0;
}
int Detecter_Mot_Reserve(char *mot){
    if(!strcmp(mot,"Select")){
        return SELECT_KW;
    }
    if(!strcmp(mot,"Where")){
        return WHERE_KW;
    }
    if(!strcmp(mot,"From")){
        return FROM_KW;
    }
    if(!strcmp(mot,"Count")){
        return COUNT_KW;
    }
    if(!strcmp(mot,"Distinct")){
        return DISTINCT_KW;
    }
    if(!strcmp(mot,"And")){
        return OPERATEUR_LOGIQUE;
    }
    if(!strcmp(mot,"Or")){
        return OPERATEUR_LOGIQUE;
    }
    return 0;
}

int AEF_SQL(char* requete){

    /*
        * Q1 : Etat finale pour reconnaitre identifiant
    
        *Q2,Q4 : Etat finale pour rec un nombre (entier/réel)
    
        Q5 : Etat finale pour rec un operateur relationnel
    */

    int n = (int)strlen(requete);
    int etat_automate=0;
    for (int i = 0; i < n; i++)
    {
        switch (etat_automate)
        {
        case 0:
            if(requete[i]== ' ' ||requete[i]=='\n' || requete[i]=='+'|| requete[i]=='-'|| requete[i]=='*'|| requete[i]=='/'|| requete[i]=='('|| requete[i]==')'|| requete[i]==','|| requete[i]==';'|| requete[i]=='=')
                etat_automate = 0;
            if(isalpha(requete[i]))
                etat_automate = 1;
            if(requete[i]=='<' || requete[i]=='>')
                etat_automate = 5;
            if(isdigit(requete[i]))
                etat_automate = 2;
            if(requete[i]=='.')
                etat_automate = 3;
            break;
        case 1:
            if(requete[i]== ' ' ||requete[i]=='\n' || requete[i]=='+'|| requete[i]=='-'|| requete[i]=='*'|| requete[i]=='/'|| requete[i]=='('|| requete[i]==')'|| requete[i]==','|| requete[i]==';'|| requete[i]=='=')
                etat_automate = 0;
            if(isalpha(requete[i]) || isdigit(requete[i]))
                etat_automate = 1;
            if(requete[i]=='<' || requete[i]=='>')
                etat_automate = 5;
            break;
        case 2:
            if(isdigit(requete[i]))
                etat_automate = 2;
            if(requete[i]== ' ' ||requete[i]=='\n' || requete[i]=='+'|| requete[i]=='-'|| requete[i]=='*'|| requete[i]=='/'|| requete[i]=='('|| requete[i]==')'|| requete[i]==','|| requete[i]==';'|| requete[i]=='=')
                etat_automate = 0;
            if(requete[i] == '.')
                etat_automate = 3;
            if(requete[i]=='<' || requete[i]=='>')
                etat_automate = 5;
            break;
        case 3:
            if(isdigit(requete[i]))
                etat_automate = 4;
            break;
        case 4:
            if(isdigit(requete[i]))
                etat_automate = 4;
            if(requete[i]== ' ' ||requete[i]=='\n' || requete[i]=='+'|| requete[i]=='-'|| requete[i]=='*'|| requete[i]=='/'|| requete[i]=='('|| requete[i]==')'|| requete[i]==','|| requete[i]==';'|| requete[i]=='=')
                etat_automate = 0;
            if(requete[i]=='<' || requete[i]=='>')
                etat_automate = 5;
            break;
        case 5:
            if(isalpha(requete[i]))
                etat_automate = 1;
            if(requete[i]== ' ' ||requete[i]=='\n' || requete[i]=='+'|| requete[i]=='-'|| requete[i]=='*'|| requete[i]=='/'|| requete[i]=='('|| requete[i]==')'|| requete[i]==','|| requete[i]==';'|| requete[i]=='=')
                etat_automate = 0;
            if(isdigit(requete[i]))
                etat_automate = 2;
            break;
        }
    }
    return etat_automate;
}

void AnalyseLex(UL* SuiteUL){
    UL* temp = SuiteUL;
    while(temp != NULL){
        if(AEF_SQL(temp->Lexeme) == 1){
            if(Detecter_Mot_Reserve(temp->Lexeme)){
                temp->Code = Detecter_Mot_Reserve(temp->Lexeme);
            }else{
                temp->Code = IDENTIFIANT_UL;
            }
        }
        if(AEF_SQL(temp->Lexeme) == 2 || AEF_SQL(temp->Lexeme) == 4){
            temp->Code = 261;
        }
        if(AEF_SQL(temp->Lexeme) == 5){
            temp->Code = 267;
        }
        if(Detecter_OP(temp->Lexeme)){
            temp->Code = Detecter_OP(temp->Lexeme);
        }
        if(Detecter_Sep(temp->Lexeme)){
            temp->Code = Detecter_Sep(temp->Lexeme);
        }
        if(Detecter_Parenthese(temp->Lexeme)){
            temp->Code = Detecter_Parenthese(temp->Lexeme);
        }
        if(temp->Code <260 || temp->Code >273){
            printf("Erreur ! %s non reconnu\n",temp->Lexeme);
            return;
        }
        temp = temp->Suivant;
    }
    free(temp);
    Afficher(SuiteUL);
}

void Decouper(char *chaine){
    int max = strlen(chaine);
    UL* SuiteUL = NULL;
    int n = 0;
    for (int i = 0; i <= max; i++)
    {
        if(chaine[i]==' ' || chaine[i]=='\n' || chaine[i]=='\t' || chaine[i]==0){
            int size = i-n;
            char *mot = malloc(sizeof(char)*(i-n));
            if(size != 0){
                strncpy(mot,chaine+n,size);
                mot[size]=0;
                SuiteUL = Ajouter_Fin(SuiteUL,mot);
            }
            n=i+1;
        }
    }
    AnalyseLex(SuiteUL);
}

int main(int argc, char const *argv[])
{
    //Decouper("Select Disctinct Age , salaire , telephone\nFrom emp\nWhere salaire >= 30000\nOr age > 25 And age < 50 ;");
    Decouper("Select Note From Semestre1 , Semestre2 ;");
    system("pause");
    //printf("%d",AEF_SQL(">="));
}