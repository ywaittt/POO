/*
Proiect POO - Tema 3 - Agenda personala

Descriere problema:
Se doreste modelarea unei agende personale in care utilizatorul isi poate organiza
categoriile, intalnirile si sarcinile zilnice. O intalnire apartine unei categorii,
are interval orar, locatie si participanti, iar o sarcina apartine unei categorii,
are termen limita, prioritate si o durata estimata.

Date de intrare:
- nume, culoare si descriere pentru o categorie
- titlu, data, ora de inceput, ora de sfarsit, locatie, participanti pentru o intalnire
- titlu, data scadenta, prioritate, durata estimata si status pentru o sarcina

In acest proiect, datele de intrare sunt generate direct in program, in main,
ca sa existe un scenariu complet de testare de la pornire.

Operatii posibile:
- schimbarea culorii unei categorii si cautarea unui cuvant-cheie in categorie
- calcularea duratei unei intalniri, mutarea ei pe alt interval si verificarea suprapunerii
- marcarea unei sarcini ca finalizata si verificarea urgentei ei
- adaugarea de categorii, intalniri si sarcini in agenda
- afisarea programului pe o zi
- detectarea conflictelor dintre intalniri
- afisarea sarcinilor urgente
- generarea unui mic raport pe categorii
*/

#include <algorithm>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

class Categorie {
private:
    std::string nume;
    std::string culoare;
    std::string descriere;

    bool contineText(const std::string& sursa, const std::string& cheie) const {
        return sursa.find(cheie) != std::string::npos;
    }

public:
    Categorie(const std::string& nume_, const std::string& culoare_, const std::string& descriere_)
        : nume(nume_), culoare(culoare_), descriere(descriere_) {
    }

    Categorie(const Categorie& alta)
        : nume(alta.nume), culoare(alta.culoare), descriere(alta.descriere) {
    }

    Categorie& operator=(const Categorie& alta) {
        if (this != &alta) {
            nume = alta.nume;
            culoare = alta.culoare;
            descriere = alta.descriere;
        }
        return *this;
    }

    ~Categorie() {
    }

    void schimbaCuloare(const std::string& culoareNoua) {
        culoare = culoareNoua;
    }

    bool contineCuvantCheie(const std::string& cheie) const {
        return contineText(nume, cheie) || contineText(descriere, cheie);
    }

    const std::string& getNume() const {
        return nume;
    }

    friend std::ostream& operator<<(std::ostream& out, const Categorie& categorie) {
        out << "Categorie{nume=\"" << categorie.nume
            << "\", culoare=\"" << categorie.culoare
            << "\", descriere=\"" << categorie.descriere << "\"}";
        return out;
    }
};

class Intalnire {
private:
    std::string titlu;
    std::string data;
    std::string oraStart;
    std::string oraSfarsit;
    std::string locatie;
    Categorie categorie;
    std::vector<std::string> participanti;

    int oraInMinute(const std::string& ora) const {
        const int ore = (ora[0] - '0') * 10 + (ora[1] - '0');
        const int minute = (ora[3] - '0') * 10 + (ora[4] - '0');
        return ore * 60 + minute;
    }

public:
    Intalnire(const std::string& titlu_,
              const std::string& data_,
              const std::string& oraStart_,
              const std::string& oraSfarsit_,
              const std::string& locatie_,
              const Categorie& categorie_,
              const std::vector<std::string>& participanti_)
        : titlu(titlu_),
          data(data_),
          oraStart(oraStart_),
          oraSfarsit(oraSfarsit_),
          locatie(locatie_),
          categorie(categorie_),
          participanti(participanti_) {
    }

    Intalnire(const Intalnire& alta)
        : titlu(alta.titlu),
          data(alta.data),
          oraStart(alta.oraStart),
          oraSfarsit(alta.oraSfarsit),
          locatie(alta.locatie),
          categorie(alta.categorie),
          participanti(alta.participanti) {
    }

    Intalnire& operator=(const Intalnire& alta) {
        if (this != &alta) {
            titlu = alta.titlu;
            data = alta.data;
            oraStart = alta.oraStart;
            oraSfarsit = alta.oraSfarsit;
            locatie = alta.locatie;
            categorie = alta.categorie;
            participanti = alta.participanti;
        }
        return *this;
    }

    ~Intalnire() {
    }

    int calculeazaDurataMinute() const {
        return oraInMinute(oraSfarsit) - oraInMinute(oraStart);
    }

    bool seSuprapuneCu(const Intalnire& alta) const {
        if (data != alta.data) {
            return false;
        }

        const int start1 = oraInMinute(oraStart);
        const int final1 = oraInMinute(oraSfarsit);
        const int start2 = oraInMinute(alta.oraStart);
        const int final2 = oraInMinute(alta.oraSfarsit);

        return std::max(start1, start2) < std::min(final1, final2);
    }

    void mutaInterval(const std::string& oraStartNoua, const std::string& oraSfarsitNoua) {
        if (oraInMinute(oraStartNoua) < oraInMinute(oraSfarsitNoua)) {
            oraStart = oraStartNoua;
            oraSfarsit = oraSfarsitNoua;
        }
    }

    bool esteInData(const std::string& dataCautata) const {
        return data == dataCautata;
    }

    const Categorie& getCategorie() const {
        return categorie;
    }

    const std::string& getTitlu() const {
        return titlu;
    }

    friend std::ostream& operator<<(std::ostream& out, const Intalnire& intalnire) {
        out << "Intalnire{titlu=\"" << intalnire.titlu
            << "\", data=\"" << intalnire.data
            << "\", interval=\"" << intalnire.oraStart << "-" << intalnire.oraSfarsit
            << "\", locatie=\"" << intalnire.locatie
            << "\", categorie=" << intalnire.categorie
            << ", participanti=[";

        for (std::size_t i = 0; i < intalnire.participanti.size(); ++i) {
            out << intalnire.participanti[i];
            if (i + 1 < intalnire.participanti.size()) {
                out << ", ";
            }
        }

        out << "]}";
        return out;
    }
};

class Sarcina {
private:
    std::string titlu;
    std::string dataScadenta;
    int prioritate;
    bool finalizata;
    int durataEstimata;
    Categorie categorie;

    int serialZi(const std::string& data) const {
        const int an = (data[0] - '0') * 1000 + (data[1] - '0') * 100 + (data[2] - '0') * 10 + (data[3] - '0');
        const int luna = (data[5] - '0') * 10 + (data[6] - '0');
        const int zi = (data[8] - '0') * 10 + (data[9] - '0');
        return an * 372 + luna * 31 + zi;
    }

public:
    Sarcina(const std::string& titlu_,
            const std::string& dataScadenta_,
            int prioritate_,
            bool finalizata_,
            int durataEstimata_,
            const Categorie& categorie_)
        : titlu(titlu_),
          dataScadenta(dataScadenta_),
          prioritate(prioritate_),
          finalizata(finalizata_),
          durataEstimata(durataEstimata_),
          categorie(categorie_) {
    }

    Sarcina(const Sarcina& alta)
        : titlu(alta.titlu),
          dataScadenta(alta.dataScadenta),
          prioritate(alta.prioritate),
          finalizata(alta.finalizata),
          durataEstimata(alta.durataEstimata),
          categorie(alta.categorie) {
    }

    Sarcina& operator=(const Sarcina& alta) {
        if (this != &alta) {
            titlu = alta.titlu;
            dataScadenta = alta.dataScadenta;
            prioritate = alta.prioritate;
            finalizata = alta.finalizata;
            durataEstimata = alta.durataEstimata;
            categorie = alta.categorie;
        }
        return *this;
    }

    ~Sarcina() {
    }

    void marcheazaFinalizata() {
        finalizata = true;
    }

    bool esteUrgenta(const std::string& dataCurenta) const {
        const int diferenta = serialZi(dataScadenta) - serialZi(dataCurenta);
        return !finalizata && diferenta >= 0 && diferenta <= 2;
    }

    int calculeazaScor() const {
        int scor = prioritate * 10 + durataEstimata / 10;
        if (!finalizata) {
            scor += 15;
        }
        return scor;
    }

    bool esteInData(const std::string& dataCautata) const {
        return dataScadenta == dataCautata;
    }

    bool esteFinalizata() const {
        return finalizata;
    }

    const Categorie& getCategorie() const {
        return categorie;
    }

    const std::string& getTitlu() const {
        return titlu;
    }

    int getDurataEstimata() const {
        return durataEstimata;
    }

    friend std::ostream& operator<<(std::ostream& out, const Sarcina& sarcina) {
        out << "Sarcina{titlu=\"" << sarcina.titlu
            << "\", dataScadenta=\"" << sarcina.dataScadenta
            << "\", prioritate=" << sarcina.prioritate
            << ", finalizata=" << (sarcina.finalizata ? "da" : "nu")
            << ", durataEstimata=" << sarcina.durataEstimata
            << ", categorie=" << sarcina.categorie << "}";
        return out;
    }
};

class Agenda {
private:
    std::string proprietar;
    std::vector<Categorie> categorii;
    std::vector<Intalnire> intalniri;
    std::vector<Sarcina> sarcini;

    bool categorieExista(const std::string& numeCategorie) const {
        for (const Categorie& categorie : categorii) {
            if (categorie.getNume() == numeCategorie) {
                return true;
            }
        }
        return false;
    }

    int minuteTotaleIntalniriDinZi(const std::string& data) const {
        int total = 0;
        for (const Intalnire& intalnire : intalniri) {
            if (intalnire.esteInData(data)) {
                total += intalnire.calculeazaDurataMinute();
            }
        }
        return total;
    }

public:
    Agenda(const std::string& proprietar_,
           const std::vector<Categorie>& categorii_,
           const std::vector<Intalnire>& intalniri_,
           const std::vector<Sarcina>& sarcini_)
        : proprietar(proprietar_),
          categorii(categorii_),
          intalniri(intalniri_),
          sarcini(sarcini_) {
    }

    Agenda(const Agenda& alta)
        : proprietar(alta.proprietar),
          categorii(alta.categorii),
          intalniri(alta.intalniri),
          sarcini(alta.sarcini) {
    }

    Agenda& operator=(const Agenda& alta) {
        if (this != &alta) {
            proprietar = alta.proprietar;
            categorii = alta.categorii;
            intalniri = alta.intalniri;
            sarcini = alta.sarcini;
        }
        return *this;
    }

    ~Agenda() {
    }

    void adaugaCategorie(const Categorie& categorie) {
        if (!categorieExista(categorie.getNume())) {
            categorii.push_back(categorie);
        }
    }

    void adaugaIntalnire(const Intalnire& intalnire) {
        intalniri.push_back(intalnire);
    }

    void adaugaSarcina(const Sarcina& sarcina) {
        sarcini.push_back(sarcina);
    }

    void afiseazaProgramPeZi(const std::string& data) const {
        std::cout << "\nProgramul din " << data << ":\n";

        std::cout << "Intalniri:\n";
        bool existaIntalniri = false;
        for (const Intalnire& intalnire : intalniri) {
            if (intalnire.esteInData(data)) {
                existaIntalniri = true;
                std::cout << "  - " << intalnire << "\n";
            }
        }
        if (!existaIntalniri) {
            std::cout << "  - nu exista intalniri in ziua asta\n";
        }

        std::cout << "Sarcini cu termen in ziua asta:\n";
        bool existaSarcini = false;
        for (const Sarcina& sarcina : sarcini) {
            if (sarcina.esteInData(data)) {
                existaSarcini = true;
                std::cout << "  - " << sarcina << "\n";
            }
        }
        if (!existaSarcini) {
            std::cout << "  - nu exista sarcini scadente azi\n";
        }

        std::cout << "Total minute ocupate in intalniri: " << minuteTotaleIntalniriDinZi(data) << "\n";
    }

    void detecteazaConflicte() const {
        std::cout << "\nConflicte intre intalniri:\n";
        bool gasit = false;

        for (std::size_t i = 0; i < intalniri.size(); ++i) {
            for (std::size_t j = i + 1; j < intalniri.size(); ++j) {
                if (intalniri[i].seSuprapuneCu(intalniri[j])) {
                    gasit = true;
                    std::cout << "  - conflict intre \"" << intalniri[i].getTitlu()
                              << "\" si \"" << intalniri[j].getTitlu() << "\"\n";
                }
            }
        }

        if (!gasit) {
            std::cout << "  - nu exista conflicte\n";
        }
    }

    void afiseazaSarciniUrgente(const std::string& dataCurenta) const {
        std::cout << "\nSarcini urgente la data " << dataCurenta << ":\n";
        bool gasit = false;

        for (const Sarcina& sarcina : sarcini) {
            if (sarcina.esteUrgenta(dataCurenta)) {
                gasit = true;
                std::cout << "  - " << sarcina << "\n";
            }
        }

        if (!gasit) {
            std::cout << "  - nicio sarcina urgenta\n";
        }
    }

    void afiseazaRaportCategorii() const {
        std::cout << "\nRaport pe categorii:\n";

        for (const Categorie& categorie : categorii) {
            int nrIntalniri = 0;
            int nrSarcini = 0;
            int minuteSarcini = 0;

            for (const Intalnire& intalnire : intalniri) {
                if (intalnire.getCategorie().getNume() == categorie.getNume()) {
                    ++nrIntalniri;
                }
            }

            for (const Sarcina& sarcina : sarcini) {
                if (sarcina.getCategorie().getNume() == categorie.getNume()) {
                    ++nrSarcini;
                    minuteSarcini += sarcina.getDurataEstimata();
                }
            }

            std::cout << "  - " << categorie.getNume()
                      << ": intalniri=" << nrIntalniri
                      << ", sarcini=" << nrSarcini
                      << ", minute estimate pentru sarcini=" << minuteSarcini << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Agenda& agenda) {
        out << "Agenda lui " << agenda.proprietar << "\n";
        out << "Categorii:\n";
        for (const Categorie& categorie : agenda.categorii) {
            out << "  - " << categorie << "\n";
        }

        out << "Intalniri:\n";
        for (const Intalnire& intalnire : agenda.intalniri) {
            out << "  - " << intalnire << "\n";
        }

        out << "Sarcini:\n";
        for (const Sarcina& sarcina : agenda.sarcini) {
            out << "  - " << sarcina << "\n";
        }

        return out;
    }
};

int main() {
    Categorie facultate("Facultate", "albastru", "cursuri, teme si proiecte");
    Categorie personal("Personal", "verde", "chestii de zi cu zi si planuri personale");
    Categorie munca("Munca", "galben", "taskuri legate de lucru");

    personal.schimbaCuloare("mov");
    facultate.contineCuvantCheie("teme");

    Intalnire labPOO(
        "Laborator POO",
        "2026-03-25",
        "10:00",
        "12:00",
        "FMI sala 214",
        facultate,
        {"Carlos", "colegii"}
    );

    Intalnire proiectEchipa(
        "Sedinta proiect",
        "2026-03-25",
        "11:30",
        "12:30",
        "Google Meet",
        facultate,
        {"Carlos", "Ana", "Vlad"}
    );

    Intalnire mersSala(
        "Sala",
        "2026-03-25",
        "18:00",
        "19:30",
        "World Class",
        personal,
        {"Carlos"}
    );

    proiectEchipa.calculeazaDurataMinute();
    labPOO.seSuprapuneCu(proiectEchipa);
    proiectEchipa.mutaInterval("12:30", "13:30");

    Sarcina temaPA(
        "Finalizat tema la PA",
        "2026-03-26",
        5,
        false,
        120,
        facultate
    );

    Sarcina cumparaturi(
        "Cumparaturi",
        "2026-03-25",
        2,
        false,
        40,
        personal
    );

    Sarcina raportClient(
        "Trimis raport client",
        "2026-03-28",
        4,
        false,
        90,
        munca
    );

    temaPA.esteUrgenta("2026-03-25");
    temaPA.calculeazaScor();
    cumparaturi.marcheazaFinalizata();

    Agenda agendaMea("Carlos", {}, {}, {});
    agendaMea.adaugaCategorie(facultate);
    agendaMea.adaugaCategorie(personal);
    agendaMea.adaugaCategorie(munca);

    agendaMea.adaugaIntalnire(labPOO);
    agendaMea.adaugaIntalnire(proiectEchipa);
    agendaMea.adaugaIntalnire(mersSala);

    agendaMea.adaugaSarcina(temaPA);
    agendaMea.adaugaSarcina(cumparaturi);
    agendaMea.adaugaSarcina(raportClient);

    std::cout << agendaMea << "\n";
    agendaMea.afiseazaProgramPeZi("2026-03-25");
    agendaMea.detecteazaConflicte();
    agendaMea.afiseazaSarciniUrgente("2026-03-25");
    agendaMea.afiseazaRaportCategorii();

    return 0;
}
