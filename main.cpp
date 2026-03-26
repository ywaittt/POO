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

In acest proiect, datele sunt generate direct in program, in main,
ca sa existe un scenariu complet de rulare de la pornire.

Operatii posibile:
- schimbarea culorii unei categorii si cautarea unui cuvant-cheie in categorie
- calcularea duratei unei intalniri, mutarea ei pe alt interval si verificarea suprapunerii
- marcarea unei sarcini ca finalizata si verificarea urgentei ei
- adaugarea de categorii, intalniri si sarcini in agenda
- afisarea programului pe o zi
- detectarea conflictelor dintre intalniri
- afisarea sarcinilor urgente
- generarea unui raport pe categorii
*/

#include <iostream>

int lungimeText(const char* text) {
    if (text == 0) {
        return 0;
    }

    int lungime = 0;
    while (text[lungime] != '\0') {
        lungime++;
    }
    return lungime;
}

void copiazaText(char* destinatie, const char* sursa) {
    if (destinatie == 0) {
        return;
    }

    if (sursa == 0) {
        destinatie[0] = '\0';
        return;
    }

    int i = 0;
    while (sursa[i] != '\0') {
        destinatie[i] = sursa[i];
        i++;
    }
    destinatie[i] = '\0';
}

bool texteEgale(const char* text1, const char* text2) {
    if (text1 == 0 && text2 == 0) {
        return true;
    }

    if (text1 == 0 || text2 == 0) {
        return false;
    }

    int i = 0;
    while (text1[i] != '\0' && text2[i] != '\0') {
        if (text1[i] != text2[i]) {
            return false;
        }
        i++;
    }

    return text1[i] == text2[i];
}

char* copieText(const char* sursa) {
    int lungime = lungimeText(sursa);
    char* destinatie = new char[lungime + 1];
    copiazaText(destinatie, sursa);
    return destinatie;
}

bool contineSubsir(const char* text, const char* cheie) {
    if (text == 0 || cheie == 0) {
        return false;
    }

    if (cheie[0] == '\0') {
        return true;
    }

    int lungimeTextTotal = lungimeText(text);
    int lungimeCheie = lungimeText(cheie);

    if (lungimeCheie > lungimeTextTotal) {
        return false;
    }

    for (int i = 0; i <= lungimeTextTotal - lungimeCheie; i++) {
        bool potrivire = true;
        for (int j = 0; j < lungimeCheie; j++) {
            if (text[i + j] != cheie[j]) {
                potrivire = false;
                break;
            }
        }

        if (potrivire) {
            return true;
        }
    }

    return false;
}

class Categorie {
private:
    char* nume;
    char* culoare;
    char* descriere;

public:
    Categorie(const char* nume_ = "", const char* culoare_ = "", const char* descriere_ = "") {
        nume = copieText(nume_);
        culoare = copieText(culoare_);
        descriere = copieText(descriere_);
    }

    Categorie(const Categorie& alta) {
        nume = copieText(alta.nume);
        culoare = copieText(alta.culoare);
        descriere = copieText(alta.descriere);
    }

    Categorie& operator=(const Categorie& alta) {
        if (this != &alta) {
            delete[] nume;
            delete[] culoare;
            delete[] descriere;

            nume = copieText(alta.nume);
            culoare = copieText(alta.culoare);
            descriere = copieText(alta.descriere);
        }
        return *this;
    }

    ~Categorie() {
        delete[] nume;
        delete[] culoare;
        delete[] descriere;
    }

    void schimbaCuloare(const char* culoareNoua) {
        delete[] culoare;
        culoare = copieText(culoareNoua);
    }

    bool contineCuvantCheie(const char* cheie) const {
        return contineSubsir(nume, cheie) || contineSubsir(descriere, cheie);
    }

    const char* getNume() const {
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
    char* titlu;
    char* data;
    char* oraStart;
    char* oraSfarsit;
    char* locatie;
    Categorie categorie;
    char** participanti;
    int nrParticipanti;

    int oraInMinute(const char* ora) const {
        int ore = (ora[0] - '0') * 10 + (ora[1] - '0');
        int minute = (ora[3] - '0') * 10 + (ora[4] - '0');
        return ore * 60 + minute;
    }

    void elibereazaParticipanti() {
        for (int i = 0; i < nrParticipanti; i++) {
            delete[] participanti[i];
        }
        delete[] participanti;
        participanti = 0;
        nrParticipanti = 0;
    }

    void copieParticipanti(const char* const* lista, int numar) {
        nrParticipanti = numar;

        if (nrParticipanti == 0) {
            participanti = 0;
            return;
        }

        participanti = new char*[nrParticipanti];
        for (int i = 0; i < nrParticipanti; i++) {
            participanti[i] = copieText(lista[i]);
        }
    }

public:
    Intalnire(const char* titlu_ = "",
              const char* data_ = "",
              const char* oraStart_ = "00:00",
              const char* oraSfarsit_ = "00:00",
              const char* locatie_ = "",
              const Categorie& categorie_ = Categorie(),
              const char* const* participanti_ = 0,
              int nrParticipanti_ = 0)
        : categorie(categorie_) {
        titlu = copieText(titlu_);
        data = copieText(data_);
        oraStart = copieText(oraStart_);
        oraSfarsit = copieText(oraSfarsit_);
        locatie = copieText(locatie_);
        participanti = 0;
        nrParticipanti = 0;
        copieParticipanti(participanti_, nrParticipanti_);
    }

    Intalnire(const Intalnire& alta)
        : categorie(alta.categorie) {
        titlu = copieText(alta.titlu);
        data = copieText(alta.data);
        oraStart = copieText(alta.oraStart);
        oraSfarsit = copieText(alta.oraSfarsit);
        locatie = copieText(alta.locatie);
        participanti = 0;
        nrParticipanti = 0;
        copieParticipanti((const char* const*)alta.participanti, alta.nrParticipanti);
    }

    Intalnire& operator=(const Intalnire& alta) {
        if (this != &alta) {
            delete[] titlu;
            delete[] data;
            delete[] oraStart;
            delete[] oraSfarsit;
            delete[] locatie;
            elibereazaParticipanti();

            titlu = copieText(alta.titlu);
            data = copieText(alta.data);
            oraStart = copieText(alta.oraStart);
            oraSfarsit = copieText(alta.oraSfarsit);
            locatie = copieText(alta.locatie);
            categorie = alta.categorie;
            copieParticipanti((const char* const*)alta.participanti, alta.nrParticipanti);
        }
        return *this;
    }

    ~Intalnire() {
        delete[] titlu;
        delete[] data;
        delete[] oraStart;
        delete[] oraSfarsit;
        delete[] locatie;
        elibereazaParticipanti();
    }

    int calculeazaDurataMinute() const {
        return oraInMinute(oraSfarsit) - oraInMinute(oraStart);
    }

    bool seSuprapuneCu(const Intalnire& alta) const {
        if (!texteEgale(data, alta.data)) {
            return false;
        }

        int start1 = oraInMinute(oraStart);
        int final1 = oraInMinute(oraSfarsit);
        int start2 = oraInMinute(alta.oraStart);
        int final2 = oraInMinute(alta.oraSfarsit);

        return !(final1 <= start2 || final2 <= start1);
    }

    void mutaInterval(const char* oraStartNoua, const char* oraSfarsitNoua) {
        if (oraInMinute(oraStartNoua) < oraInMinute(oraSfarsitNoua)) {
            delete[] oraStart;
            delete[] oraSfarsit;
            oraStart = copieText(oraStartNoua);
            oraSfarsit = copieText(oraSfarsitNoua);
        }
    }

    bool esteInData(const char* dataCautata) const {
        return texteEgale(data, dataCautata);
    }

    const Categorie& getCategorie() const {
        return categorie;
    }

    const char* getTitlu() const {
        return titlu;
    }

    friend std::ostream& operator<<(std::ostream& out, const Intalnire& intalnire) {
        out << "Intalnire{titlu=\"" << intalnire.titlu
            << "\", data=\"" << intalnire.data
            << "\", interval=\"" << intalnire.oraStart << "-" << intalnire.oraSfarsit
            << "\", locatie=\"" << intalnire.locatie
            << "\", categorie=" << intalnire.categorie
            << ", participanti=[";

        for (int i = 0; i < intalnire.nrParticipanti; i++) {
            out << intalnire.participanti[i];
            if (i < intalnire.nrParticipanti - 1) {
                out << ", ";
            }
        }

        out << "]}";
        return out;
    }
};

class Sarcina {
private:
    char* titlu;
    char* dataScadenta;
    int prioritate;
    bool finalizata;
    int durataEstimata;
    Categorie categorie;

    int serialZi(const char* data) const {
        int an = (data[0] - '0') * 1000 + (data[1] - '0') * 100 + (data[2] - '0') * 10 + (data[3] - '0');
        int luna = (data[5] - '0') * 10 + (data[6] - '0');
        int zi = (data[8] - '0') * 10 + (data[9] - '0');
        return an * 372 + luna * 31 + zi;
    }

public:
    Sarcina(const char* titlu_ = "",
            const char* dataScadenta_ = "",
            int prioritate_ = 1,
            bool finalizata_ = false,
            int durataEstimata_ = 0,
            const Categorie& categorie_ = Categorie())
        : categorie(categorie_) {
        titlu = copieText(titlu_);
        dataScadenta = copieText(dataScadenta_);
        prioritate = prioritate_;
        finalizata = finalizata_;
        durataEstimata = durataEstimata_;
    }

    Sarcina(const Sarcina& alta)
        : categorie(alta.categorie) {
        titlu = copieText(alta.titlu);
        dataScadenta = copieText(alta.dataScadenta);
        prioritate = alta.prioritate;
        finalizata = alta.finalizata;
        durataEstimata = alta.durataEstimata;
    }

    Sarcina& operator=(const Sarcina& alta) {
        if (this != &alta) {
            delete[] titlu;
            delete[] dataScadenta;

            titlu = copieText(alta.titlu);
            dataScadenta = copieText(alta.dataScadenta);
            prioritate = alta.prioritate;
            finalizata = alta.finalizata;
            durataEstimata = alta.durataEstimata;
            categorie = alta.categorie;
        }
        return *this;
    }

    ~Sarcina() {
        delete[] titlu;
        delete[] dataScadenta;
    }

    void marcheazaFinalizata() {
        finalizata = true;
    }

    bool esteUrgenta(const char* dataCurenta) const {
        int diferenta = serialZi(dataScadenta) - serialZi(dataCurenta);
        return !finalizata && diferenta >= 0 && diferenta <= 2;
    }

    int calculeazaScor() const {
        int scor = prioritate * 10 + durataEstimata / 10;
        if (!finalizata) {
            scor += 15;
        }
        return scor;
    }

    bool esteInData(const char* dataCautata) const {
        return texteEgale(dataScadenta, dataCautata);
    }

    bool esteFinalizata() const {
        return finalizata;
    }

    const Categorie& getCategorie() const {
        return categorie;
    }

    const char* getTitlu() const {
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
    char* proprietar;

    Categorie* categorii;
    int nrCategorii;
    int capacitateCategorii;

    Intalnire* intalniri;
    int nrIntalniri;
    int capacitateIntalniri;

    Sarcina* sarcini;
    int nrSarcini;
    int capacitateSarcini;

    void redimensioneazaCategorii() {
        capacitateCategorii = capacitateCategorii * 2;
        Categorie* nou = new Categorie[capacitateCategorii];
        for (int i = 0; i < nrCategorii; i++) {
            nou[i] = categorii[i];
        }
        delete[] categorii;
        categorii = nou;
    }

    void redimensioneazaIntalniri() {
        capacitateIntalniri = capacitateIntalniri * 2;
        Intalnire* nou = new Intalnire[capacitateIntalniri];
        for (int i = 0; i < nrIntalniri; i++) {
            nou[i] = intalniri[i];
        }
        delete[] intalniri;
        intalniri = nou;
    }

    void redimensioneazaSarcini() {
        capacitateSarcini = capacitateSarcini * 2;
        Sarcina* nou = new Sarcina[capacitateSarcini];
        for (int i = 0; i < nrSarcini; i++) {
            nou[i] = sarcini[i];
        }
        delete[] sarcini;
        sarcini = nou;
    }

    bool categorieExista(const char* numeCategorie) const {
        for (int i = 0; i < nrCategorii; i++) {
            if (texteEgale(categorii[i].getNume(), numeCategorie)) {
                return true;
            }
        }
        return false;
    }

    int minuteTotaleIntalniriDinZi(const char* data) const {
        int total = 0;
        for (int i = 0; i < nrIntalniri; i++) {
            if (intalniri[i].esteInData(data)) {
                total += intalniri[i].calculeazaDurataMinute();
            }
        }
        return total;
    }

public:
    Agenda(const char* proprietar_ = "") {
        proprietar = copieText(proprietar_);

        nrCategorii = 0;
        capacitateCategorii = 5;
        categorii = new Categorie[capacitateCategorii];

        nrIntalniri = 0;
        capacitateIntalniri = 5;
        intalniri = new Intalnire[capacitateIntalniri];

        nrSarcini = 0;
        capacitateSarcini = 5;
        sarcini = new Sarcina[capacitateSarcini];
    }

    Agenda(const Agenda& alta) {
        proprietar = copieText(alta.proprietar);

        nrCategorii = alta.nrCategorii;
        capacitateCategorii = alta.capacitateCategorii;
        categorii = new Categorie[capacitateCategorii];
        for (int i = 0; i < nrCategorii; i++) {
            categorii[i] = alta.categorii[i];
        }

        nrIntalniri = alta.nrIntalniri;
        capacitateIntalniri = alta.capacitateIntalniri;
        intalniri = new Intalnire[capacitateIntalniri];
        for (int i = 0; i < nrIntalniri; i++) {
            intalniri[i] = alta.intalniri[i];
        }

        nrSarcini = alta.nrSarcini;
        capacitateSarcini = alta.capacitateSarcini;
        sarcini = new Sarcina[capacitateSarcini];
        for (int i = 0; i < nrSarcini; i++) {
            sarcini[i] = alta.sarcini[i];
        }
    }

    Agenda& operator=(const Agenda& alta) {
        if (this != &alta) {
            delete[] proprietar;
            delete[] categorii;
            delete[] intalniri;
            delete[] sarcini;

            proprietar = copieText(alta.proprietar);

            nrCategorii = alta.nrCategorii;
            capacitateCategorii = alta.capacitateCategorii;
            categorii = new Categorie[capacitateCategorii];
            for (int i = 0; i < nrCategorii; i++) {
                categorii[i] = alta.categorii[i];
            }

            nrIntalniri = alta.nrIntalniri;
            capacitateIntalniri = alta.capacitateIntalniri;
            intalniri = new Intalnire[capacitateIntalniri];
            for (int i = 0; i < nrIntalniri; i++) {
                intalniri[i] = alta.intalniri[i];
            }

            nrSarcini = alta.nrSarcini;
            capacitateSarcini = alta.capacitateSarcini;
            sarcini = new Sarcina[capacitateSarcini];
            for (int i = 0; i < nrSarcini; i++) {
                sarcini[i] = alta.sarcini[i];
            }
        }
        return *this;
    }

    ~Agenda() {
        delete[] proprietar;
        delete[] categorii;
        delete[] intalniri;
        delete[] sarcini;
    }

    void adaugaCategorie(const Categorie& categorie) {
        if (!categorieExista(categorie.getNume())) {
            if (nrCategorii == capacitateCategorii) {
                redimensioneazaCategorii();
            }
            categorii[nrCategorii] = categorie;
            nrCategorii++;
        }
    }

    void adaugaIntalnire(const Intalnire& intalnire) {
        if (nrIntalniri == capacitateIntalniri) {
            redimensioneazaIntalniri();
        }
        intalniri[nrIntalniri] = intalnire;
        nrIntalniri++;
    }

    void adaugaSarcina(const Sarcina& sarcina) {
        if (nrSarcini == capacitateSarcini) {
            redimensioneazaSarcini();
        }
        sarcini[nrSarcini] = sarcina;
        nrSarcini++;
    }

    void afiseazaProgramPeZi(const char* data) const {
        std::cout << "\nProgramul din " << data << ":\n";

        std::cout << "Intalniri:\n";
        bool existaIntalniri = false;
        for (int i = 0; i < nrIntalniri; i++) {
            if (intalniri[i].esteInData(data)) {
                existaIntalniri = true;
                std::cout << "  - " << intalniri[i] << "\n";
            }
        }
        if (!existaIntalniri) {
            std::cout << "  - nu exista intalniri in ziua asta\n";
        }

        std::cout << "Sarcini cu termen in ziua asta:\n";
        bool existaSarcini = false;
        for (int i = 0; i < nrSarcini; i++) {
            if (sarcini[i].esteInData(data)) {
                existaSarcini = true;
                std::cout << "  - " << sarcini[i] << "\n";
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

        for (int i = 0; i < nrIntalniri; i++) {
            for (int j = i + 1; j < nrIntalniri; j++) {
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

    void afiseazaSarciniUrgente(const char* dataCurenta) const {
        std::cout << "\nSarcini urgente la data " << dataCurenta << ":\n";
        bool gasit = false;

        for (int i = 0; i < nrSarcini; i++) {
            if (sarcini[i].esteUrgenta(dataCurenta)) {
                gasit = true;
                std::cout << "  - " << sarcini[i] << "\n";
            }
        }

        if (!gasit) {
            std::cout << "  - nicio sarcina urgenta\n";
        }
    }

    void afiseazaRaportCategorii() const {
        std::cout << "\nRaport pe categorii:\n";

        for (int i = 0; i < nrCategorii; i++) {
            int nrIntalniriCategorie = 0;
            int nrSarciniCategorie = 0;
            int minuteSarcini = 0;

            for (int j = 0; j < nrIntalniri; j++) {
                if (texteEgale(intalniri[j].getCategorie().getNume(), categorii[i].getNume())) {
                    nrIntalniriCategorie++;
                }
            }

            for (int j = 0; j < nrSarcini; j++) {
                if (texteEgale(sarcini[j].getCategorie().getNume(), categorii[i].getNume())) {
                    nrSarciniCategorie++;
                    minuteSarcini += sarcini[j].getDurataEstimata();
                }
            }

            std::cout << "  - " << categorii[i].getNume()
                      << ": intalniri=" << nrIntalniriCategorie
                      << ", sarcini=" << nrSarciniCategorie
                      << ", minute estimate pentru sarcini=" << minuteSarcini << "\n";
        }
    }

    friend std::ostream& operator<<(std::ostream& out, const Agenda& agenda) {
        out << "Agenda lui " << agenda.proprietar << "\n";
        out << "Categorii:\n";
        for (int i = 0; i < agenda.nrCategorii; i++) {
            out << "  - " << agenda.categorii[i] << "\n";
        }

        out << "Intalniri:\n";
        for (int i = 0; i < agenda.nrIntalniri; i++) {
            out << "  - " << agenda.intalniri[i] << "\n";
        }

        out << "Sarcini:\n";
        for (int i = 0; i < agenda.nrSarcini; i++) {
            out << "  - " << agenda.sarcini[i] << "\n";
        }

        return out;
    }
};

int main() {
    Categorie facultate("Facultate", "albastru", "cursuri, teme si proiecte");
    Categorie personal("Personal", "verde", "chestii de zi cu zi si planuri personale");
    Categorie munca("Munca", "galben", "taskuri legate de lucru");

    personal.schimbaCuloare("mov");

    if (facultate.contineCuvantCheie("teme")) {
        std::cout << "Categoria " << facultate.getNume() << " contine cuvantul cautat.\n";
    }

    Categorie copieCategorie(facultate);
    Categorie categorieAsignata;
    categorieAsignata = personal;

    const char* participanti1[] = {"Carlos", "colegii"};
    const char* participanti2[] = {"Carlos", "Ana", "Vlad"};
    const char* participanti3[] = {"Carlos", "client"};
    const char* participanti4[] = {"Carlos"};

    Intalnire labPOO(
        "Laborator POO",
        "2026-03-25",
        "10:00",
        "12:00",
        "FMI sala 214",
        facultate,
        participanti1,
        2
    );

    Intalnire proiectEchipa(
        "Sedinta proiect",
        "2026-03-25",
        "11:30",
        "12:30",
        "Google Meet",
        facultate,
        participanti2,
        3
    );

    Intalnire callClient(
        "Call client",
        "2026-03-25",
        "18:00",
        "19:00",
        "Birou",
        munca,
        participanti3,
        2
    );

    Intalnire mersSala(
        "Sala",
        "2026-03-25",
        "18:30",
        "20:00",
        "World Class",
        personal,
        participanti4,
        1
    );

    std::cout << "Durata pentru " << labPOO.getTitlu() << " este de "
              << labPOO.calculeazaDurataMinute() << " minute.\n";

    if (labPOO.seSuprapuneCu(proiectEchipa)) {
        std::cout << "Intalnirea de proiect se suprapunea initial cu laboratorul, asa ca a fost mutata.\n";
        proiectEchipa.mutaInterval("12:30", "13:30");
    }

    Intalnire copieIntalnire(labPOO);
    Intalnire intalnireAsignata;
    intalnireAsignata = proiectEchipa;

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

    std::cout << "Scor pentru sarcina \"" << temaPA.getTitlu() << "\" este "
              << temaPA.calculeazaScor() << ".\n";

    if (temaPA.esteUrgenta("2026-03-25")) {
        std::cout << "Sarcina " << temaPA.getTitlu() << " este urgenta.\n";
    }

    cumparaturi.marcheazaFinalizata();
    if (cumparaturi.esteFinalizata()) {
        std::cout << "Sarcina " << cumparaturi.getTitlu() << " a fost marcata ca finalizata.\n";
    }

    std::cout << "Categoria sarcinii de raport este "
              << raportClient.getCategorie().getNume() << ", iar durata estimata este "
              << raportClient.getDurataEstimata() << " minute.\n";

    Sarcina copieSarcina(temaPA);
    Sarcina sarcinaAsignata;
    sarcinaAsignata = raportClient;

    Agenda agendaCarlos("Carlos");
    agendaCarlos.adaugaCategorie(facultate);
    agendaCarlos.adaugaCategorie(personal);
    agendaCarlos.adaugaCategorie(munca);

    agendaCarlos.adaugaIntalnire(labPOO);
    agendaCarlos.adaugaIntalnire(proiectEchipa);
    agendaCarlos.adaugaIntalnire(callClient);
    agendaCarlos.adaugaIntalnire(mersSala);

    agendaCarlos.adaugaSarcina(temaPA);
    agendaCarlos.adaugaSarcina(cumparaturi);
    agendaCarlos.adaugaSarcina(raportClient);

    Agenda agendaCopie(agendaCarlos);
    Agenda agendaAsignata;
    agendaAsignata = agendaCarlos;

    std::cout << "\n" << agendaCarlos;
    agendaCarlos.afiseazaProgramPeZi("2026-03-25");
    agendaCarlos.detecteazaConflicte();
    agendaCarlos.afiseazaSarciniUrgente("2026-03-25");
    agendaCarlos.afiseazaRaportCategorii();

    std::cout << "\nCopiile facute pentru obiecte exista in program si au fost create corect.\n";
    std::cout << copieCategorie << "\n";
    std::cout << copieIntalnire << "\n";
    std::cout << copieSarcina << "\n";
    std::cout << agendaCopie;
    std::cout << agendaAsignata;
    std::cout << categorieAsignata << "\n";
    std::cout << intalnireAsignata << "\n";
    std::cout << sarcinaAsignata << "\n";

    return 0;
}
