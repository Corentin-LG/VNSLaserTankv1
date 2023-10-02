#include <stdio.h>
#include <wchar.h>
#include <string.h>

struct ConversionTable
{
    wchar_t* lettre;
    int valeur;
};

struct ConversionTable table[] = {
    {L"Ax", 1}, {L"bc", 2}, {L"Urr", 3}, {L"tr", 2}
};

int main()
{
    wchar_t chainInput[] = L"bc Urr Urr tr bc Ax";
    int intOutput[100]; // Vous pouvez ajuster la taille selon vos besoins

    // Initialiser l'indice pour intOutput
    int outputIndex = 0;

    // Découper la chaîne en mots
    wchar_t* token = wcstok(chainInput, L" ");
    while (token != NULL)
    {
        // Parcourir la table de conversion pour trouver la correspondance
        for (int i = 0; i < 4; i++)
        {
            if (wcscmp(token, table[i].lettre) == 0)
            {
                intOutput[outputIndex++] = table[i].valeur;
                break;
            }
        }

        // Obtenir le mot suivant
        token = wcstok(NULL, L" ");
    }

    // Afficher les numéros correspondants
    printf("Numéros correspondants : ");
    for (int i = 0; i < outputIndex; i++)
    {
        printf("%d ", intOutput[i]);
    }
    printf("\n");

    return 0;
}
