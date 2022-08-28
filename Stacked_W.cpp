#include "Stacked_W.h"
#include "Main_MPW.h"
#include "Rsa_MPW.h"
#include "deffiehellman_MPW.h"
#include "shamir_mpw.h"
#include "Elgamal_MPW.h"
#include "Hash_MPW.h"
#include "Rsa_eds_MPW.h"
#include "Elgamal_eds_MPW.h"
#include "Gost3410_12_MPW.h"

Stacked_W::Stacked_W()
{
    Rsa_MPW *rsa_page = new Rsa_MPW;
    DeffieHellman_MPW *dh_page = new DeffieHellman_MPW;
    Shamir_MPW *shamir_page = new Shamir_MPW;
    Elgamal_MPW *elgamal_page = new Elgamal_MPW;
    Hash_MPW *hashes_page = new Hash_MPW;
    Rsa_eds_MPW *rsa_eds_page = new Rsa_eds_MPW;
    Elgamal_eds_MPW *elgamal_eds_page = new Elgamal_eds_MPW;
    Gost3410_12_MPW *gost_eds_page = new Gost3410_12_MPW;

    setTabPosition(QTabWidget::West);
    tabBar()->setStyle(new CustomTabStyle);

    addTab(rsa_page,"RSA");
    addTab(dh_page,"Deffie-Hellman");
    addTab(shamir_page,"Shamir");
    addTab(elgamal_page,"Elgamal");
    addTab(hashes_page,"Hashes");
    addTab(rsa_eds_page,"Rsa ЭП");
    addTab(elgamal_eds_page,"Elgamal ЭП");
    addTab(gost_eds_page,"ГОСТ 34.10-2012 512bit");
}

