/*******************************************************************************
* This file is part of Tihu.
*
* Tihu is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the License, or
* (at your option) any later version.
*
* Tihu is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with Tihu.  If not, see <http://www.gnu.org/licenses/>.
*
* Contributor(s):
*    Mostafa Sedaghat Joo (mostafa.sedaghat@gmail.com)
*
*******************************************************************************/
#include "afx_manager.h"
#include "hash_manager.h"
#include "afx_entry.h"
#include "pfx_entry.h"
#include "sfx_entry.h"


CAfxManager::CAfxManager(CHashManager* hash_manager)
    : HashManager(hash_manager)
{
    for(int i = 0; i < SETSIZE; i++) {
        PrefixStart[i] = NULL;
        SuffixStart[i] = NULL;
        PrefixFlag[i] = NULL;
        SuffixFlag[i] = NULL;
    }
}

CAfxManager::~CAfxManager()
{
}

bool CAfxManager::Load(std::string filename)
{
    CFileManager file_manager;

    if(file_manager.OpenFile(filename)) {
        return false;
    }

    while(file_manager.ReadLine()) {
        /// parse this affix: P - prefix, S - suffix
        std::string piece = file_manager.NextPiece();
        if(piece.compare(0, 3, "PFX", 3) == 0 ||
           piece.compare(0, 3, "SFX", 3) == 0) {
            ParseAffix(piece[0], file_manager);
        }
    }

    // convert affix trees to sorted list
    ProcessPfxTreeToList();
    ProcessSfxTreeToList();

    ProcessPrefixOrder();
    ProcessSuffixOrder();

    return true;
}

int CAfxManager::ParseAffix(char afx_type, CFileManager &file_manager)
{
    std::string flag_s = file_manager.NextPiece();
    int numents = std::stoi(file_manager.NextPiece());

    int flag = ((unsigned short)flag_s[0] << 8) + (unsigned short)flag_s[1];

    if((numents <= 0) || ((std::numeric_limits<size_t>::max() /
                           sizeof(CAfxEntry)) < static_cast<size_t>(numents))) {
        return -1;
    }

    std::vector<CAfxEntry*> entries;

    entries.reserve(numents);

    // now parse numents affentries for this affix
    for(int ent = 0; ent < numents; ++ent) {
        file_manager.ReadLine();

        std::string morph_type      = file_manager.NextPiece();
        std::string morph_flag      = file_manager.NextPiece();
        std::string morph_append    = file_manager.NextPiece();
        std::string morph_label     = file_manager.NextPiece();
        std::string morph_pronounce = file_manager.NextPiece();

        if(morph_label.empty()) {
            TIHU_WARNING(stderr, "error: line %d: wrong affix entry.",
                         file_manager.GetLineNum());
            break;
        }

        CAfxEntry* afx_entry = nullptr;
        if(afx_type == 'S') {
            afx_entry = new CSfxEntry(this);
        } else {
            afx_entry = new CPfxEntry(this);
        }

        ///
        ReplaceSubstring(morph_append, "_", CHR_U8_ZWNJ);

        afx_entry->Flag      = flag;
        afx_entry->Appnd     = morph_append;
        afx_entry->Pronounce = morph_pronounce;
        afx_entry->Label     = morph_label;

        entries.push_back(afx_entry);
    }

    // now create SfxEntry or PfxEntry objects and use links to
    // build an ordered (sorted by affix string) list
    for(auto &entry : entries) {
        if(afx_type == 'P') {
            BuildPrefixTree(static_cast<CPfxEntry*>(entry));
        } else {
            BuildSuffixTree(static_cast<CSfxEntry*>(entry));
        }
    }

    return 0;
}


// we want to be able to quickly access prefix information
// both by prefix flag, and sorted by prefix string itself
// so we need to set up two indexes
int CAfxManager::BuildPrefixTree(CPfxEntry* pfxptr)
{
    CPfxEntry* ptr;
    CPfxEntry* pptr;
    CPfxEntry* ep = pfxptr;

    // get the right starting points
    const char* key = ep->GetKey();
    const unsigned char flg = (unsigned char)(ep->GetFlag() & 0x00FF);

    // first index by flag which must exist
    ptr = PrefixFlag[flg];
    PrefixFlag[flg] = ep;

    // now handle the normal case
    ep->SetNextEQ(NULL);
    ep->SetNextNE(NULL);

    // next index by affix string
    unsigned char sp = *((const unsigned char*)key);
    ptr = PrefixStart[sp];

    // handle the first insert
    if(!ptr) {
        PrefixStart[sp] = ep;
        return 0;
    }

    // otherwise use binary tree insertion so that a sorted
    // list can easily be generated later
    pptr = NULL;
    for(;;) {
        pptr = ptr;
        if(strcmp(ep->GetKey(), ptr->GetKey()) <= 0) {
            ptr = ptr->GetNextEQ();
            if(!ptr) {
                pptr->SetNextEQ(ep);
                break;
            }
        } else {
            ptr = ptr->GetNextNE();
            if(!ptr) {
                pptr->SetNextNE(ep);
                break;
            }
        }
    }
    return 0;
}

// we want to be able to quickly access suffix information
// both by suffix flag, and sorted by the reverse of the
// suffix string itself; so we need to set up two indexes
int CAfxManager::BuildSuffixTree(CSfxEntry* sfxptr)
{
    sfxptr->initReverseWord();

    CSfxEntry* ptr;
    CSfxEntry* pptr;
    CSfxEntry* ep = sfxptr;

    /* get the right starting point */
    const char* key = ep->GetKey();
    const unsigned char flg = (unsigned char)(ep->GetFlag() & 0x00FF);

    // first index by flag which must exist
    ptr = SuffixFlag[flg];
    SuffixFlag[flg] = ep;


    // now handle the normal case
    ep->SetNextEQ(NULL);
    ep->SetNextNE(NULL);

    // next index by affix string
    unsigned char sp = *((const unsigned char*)key);
    ptr = SuffixStart[sp];

    // handle the first insert
    if(!ptr) {
        SuffixStart[sp] = ep;
        return 0;
    }

    // otherwise use binary tree insertion so that a sorted
    // list can easily be generated later
    pptr = NULL;
    for(;;) {
        pptr = ptr;
        if(strcmp(ep->GetKey(), ptr->GetKey()) <= 0) {
            ptr = ptr->GetNextEQ();
            if(!ptr) {
                pptr->SetNextEQ(ep);
                break;
            }
        } else {
            ptr = ptr->GetNextNE();
            if(!ptr) {
                pptr->SetNextNE(ep);
                break;
            }
        }
    }
    return 0;
}

// convert from binary tree to sorted list
int CAfxManager::ProcessPfxTreeToList()
{
    for(int i = 1; i < SETSIZE; i++) {
        PrefixStart[i] = ProcessPfxInOrder(PrefixStart[i], NULL);
    }
    return 0;
}

CPfxEntry* CAfxManager::ProcessPfxInOrder(CPfxEntry* ptr, CPfxEntry* nptr)
{
    if(ptr) {
        nptr = ProcessPfxInOrder(ptr->GetNextNE(), nptr);
        ptr->SetNext(nptr);
        nptr = ProcessPfxInOrder(ptr->GetNextEQ(), ptr);
    }
    return nptr;
}

// convert from binary tree to sorted list
int CAfxManager::ProcessSfxTreeToList()
{
    for(int i = 1; i < SETSIZE; i++) {
        SuffixStart[i] = ProcessSfxInOrder(SuffixStart[i], NULL);
    }
    return 0;
}

CSfxEntry* CAfxManager::ProcessSfxInOrder(CSfxEntry* ptr, CSfxEntry* nptr)
{
    if(ptr) {
        nptr = ProcessSfxInOrder(ptr->GetNextNE(), nptr);
        ptr->SetNext(nptr);
        nptr = ProcessSfxInOrder(ptr->GetNextEQ(), ptr);
    }
    return nptr;
}

// reinitialize the PfxEntry links NextEQ and NextNE to speed searching
// using the idea of leading subsets this time
int CAfxManager::ProcessPrefixOrder()
{
    CPfxEntry* ptr;

    // loop through each prefix list starting point
    for(int i = 1; i < SETSIZE; i++) {
        ptr = PrefixStart[i];

        // look through the remainder of the list
        //  and find next entry with affix that
        // the current one is not a subset of
        // mark that as destination for NextNE
        // use next in list that you are a subset
        // of as NextEQ

        for(; ptr != NULL; ptr = ptr->GetNext()) {
            CPfxEntry* nptr = ptr->GetNext();
            for(; nptr != NULL; nptr = nptr->GetNext()) {
                if(!isSubset(ptr->GetKey(), nptr->GetKey())) {
                    break;
                }
            }
            ptr->SetNextNE(nptr);
            ptr->SetNextEQ(NULL);
            if((ptr->GetNext()) &&
               isSubset(ptr->GetKey(), (ptr->GetNext())->GetKey())) {
                ptr->SetNextEQ(ptr->GetNext());
            }
        }

        // now clean up by adding smart search termination strings:
        // if you are already a superset of the previous prefix
        // but not a subset of the next, search can end here
        // so set NextNE properly

        ptr = PrefixStart[i];
        for(; ptr != NULL; ptr = ptr->GetNext()) {
            CPfxEntry* nptr = ptr->GetNext();
            CPfxEntry* mptr = NULL;
            for(; nptr != NULL; nptr = nptr->GetNext()) {
                if(!isSubset(ptr->GetKey(), nptr->GetKey())) {
                    break;
                }
                mptr = nptr;
            }
            if(mptr) {
                mptr->SetNextNE(NULL);
            }
        }
    }
    return 0;
}

// initialize the SfxEntry links NextEQ and NextNE to speed searching
// using the idea of leading subsets this time
int CAfxManager::ProcessSuffixOrder()
{
    CSfxEntry* ptr;

    // loop through each prefix list starting point
    for(int i = 1; i < SETSIZE; i++) {
        ptr = SuffixStart[i];

        // look through the remainder of the list
        //  and find next entry with affix that
        // the current one is not a subset of
        // mark that as destination for NextNE
        // use next in list that you are a subset
        // of as NextEQ

        for(; ptr != NULL; ptr = ptr->GetNext()) {
            CSfxEntry* nptr = ptr->GetNext();
            for(; nptr != NULL; nptr = nptr->GetNext()) {
                if(!isSubset(ptr->GetKey(), nptr->GetKey())) {
                    break;
                }
            }
            ptr->SetNextNE(nptr);
            ptr->SetNextEQ(NULL);
            if((ptr->GetNext()) &&
               isSubset(ptr->GetKey(), (ptr->GetNext())->GetKey())) {
                ptr->SetNextEQ(ptr->GetNext());
            }
        }

        // now clean up by adding smart search termination strings:
        // if you are already a superset of the previous suffix
        // but not a subset of the next, search can end here
        // so set NextNE properly

        ptr = SuffixStart[i];
        for(; ptr != NULL; ptr = ptr->GetNext()) {
            CSfxEntry* nptr = ptr->GetNext();
            CSfxEntry* mptr = NULL;
            for(; nptr != NULL; nptr = nptr->GetNext()) {
                if(!isSubset(ptr->GetKey(), nptr->GetKey())) {
                    break;
                }
                mptr = nptr;
            }
            if(mptr) {
                mptr->SetNextNE(NULL);
            }
        }
    }
    return 0;
}



// return 1 if s1 is a leading subset of s2
inline int CAfxManager::isSubset(const char* s1, const char* s2)
{
    while((*s1 == *s2) && (*s1 != '\0')) {
        s1++;
        s2++;
    }
    return (*s1 == '\0');
}

inline int CAfxManager::isRevSubset(const char* s1,
                                    const char* end_of_s2,
                                    int len)
{
    while((len > 0) && (*s1 != '\0') && ((*s1 == *end_of_s2) || (*s1 == '.'))) {
        s1++;
        end_of_s2--;
        len--;
    }
    return (*s1 == '\0');
}

void CAfxManager::AffixCheck(const char* text, int len,
                             const FLAG needflag, CWordPtr &word)
{
    // check all prefixes (also crossed with suffixes if allowed)
    PrefixCheck(text, len, needflag, word);

    // if still not found check all suffixes
    SuffixCheck(text, len, FLAG_NULL, needflag, word);
}


// check text for prefixes
void CAfxManager::PrefixCheck(const char* text, int len,
                              const FLAG needflag, CWordPtr &word)
{
    // now handle the general case
    unsigned char sp = *((const unsigned char*)text);
    CPfxEntry* pfx = PrefixStart[sp];

    while(pfx) {
        if(isSubset(pfx->GetKey(), text)) {

            // check prefix
            pfx->CheckWord(text, len, needflag, word);

            pfx = pfx->GetNextEQ();
        } else {
            pfx = pfx->GetNextNE();
        }
    }
}


// check text for suffixes
void CAfxManager::SuffixCheck(const char* text, int len,
                              CPfxEntry* pfx,
                              const FLAG needflag, CWordPtr &word)
{
    if(len == 0) {
        return ;    // FULLSTRIP
    }

    unsigned char sp = *((const unsigned char*)(text + len - 1));
    CSfxEntry* sfx = SuffixStart[sp];

    while(sfx) {
        if(isRevSubset(sfx->GetKey(), text + len - 1, len)) {
            // suffixes are not allowed in beginning of compounds

            sfx->CheckWord(text, len, pfx, needflag, word);

            sfx = sfx->GetNextEQ();
        } else {
            sfx = sfx->GetNextNE();
        }
    }
}


// utility method to look up root words in hash table
struct hentry* CAfxManager::Lookup(const char* text)
{
    return HashManager->Lookup(text);
}

bool CAfxManager::IsPrefix(const char* prefix)
{

    // now handle the general case
    unsigned char sp = *((const unsigned char*)prefix);
    CPfxEntry* pptr = PrefixStart[sp];

    while(pptr) {
        if(isSubset(pptr->GetKey(), prefix)) {

            return true;
        } else {
            pptr = pptr->GetNextNE();
        }
    }

    return false;
}

void CAfxManager::ParsEntry(struct hentry* he, CPfxEntry* pfx, CSfxEntry* sfx, CWordPtr &word)
{
    std::string root = he->text;
    std::string dictation = he->text;
    std::string lable = he->text + he->w_len + 1;
    std::string pronounce = he->text + he->w_len + he->l_len + 2;
    int frequency = he->freq;

    if(pfx) {
        dictation = pfx->Appnd + dictation;
        if(pfx->Label != ".") {
            if (pfx->Label[0] == '*') {
                std::string attr = pfx->Label.substr(1);

                if (lable.find(attr) == std::string::npos) {
                    lable.append("_");
                    lable.append(attr);
                }
            } else {
                lable = pfx->Label;
            }
        }
        pronounce = ConcatPronunciations(pfx->Pronounce, pronounce);
    }

    if(sfx) {
        dictation = dictation + sfx->Appnd;
        if(sfx->Label != ".") {
            lable = sfx->Label;
        }
        pronounce = ConcatPronunciations(pronounce, sfx->Pronounce);
    }

    if(!lable.empty() && !word->GetPOSTag().empty()) {
        if(lable[0] == word->GetPOSTag()[0]) {
            word->SetPron(pronounce);
            word->SetFrequency(frequency);
        }
    }
}
