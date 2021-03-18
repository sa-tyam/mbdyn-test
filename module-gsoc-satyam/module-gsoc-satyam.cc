/*  
    Hello team,
    I am Satyam, second year Computer Science and Engineering student at National 
    Institute of Technology, Hamirpur, India.
    I am determined to contribute to the MBDyn community and planning to 
    participate in GSOC-2021 by applying for MBDyn community.
    
    For my entry test I planned to use the relation between 
    - strain (Eps)
    - rigidity modulud (G)
    - Poisson's ratio (Mu)
    - force  (F)
    to create this module. 
    
    the mathematical formulae used are 
    
    force (F) = 2 * G * ( 1 + Mu ) * Eps                    (considering unit square surface area)

    In order to prepare this module I took references from the 'module-constlaw', 
    'module-cont-contact' and 'module-hunt-crossley' present in the module directory.

*/

#include "mbconfig.h" 

#include <cmath>  
#include <cfloat>

#include "dataman.h"
#include "constltp.h"

/* creating class for only 1D objects considering it has a unit square surface area for easier calculations */

class GsocSatyamClass
: public ConstitutiveLaw <doublereal, doublereal> {
    
    private:
        doublereal rigid_mod;              /* rigidity modulud (G) */ 
        doublereal poisson_ratio;           /* poisson's ratio (Mu) */

    public:
        GsocSatyamClass(doublereal rmod, doublereal mu)
        : rigid_mod(rmod), poisson_ratio(mu)
        {
            ConstitutiveLaw<doublereal, doublereal>::FDE = mb_deye<doublereal>(2 * rigid_mod * ( 1 + poisson_ratio ));

            /* using the formula (rigidity constant) E = 2 x G x ( 1 + Mu ) 
                and for unit square surface area stiffness = rigidity constant = force strain derivative */
        };

        virtual ~GsocSatyamClass(void)
        {
            NO_OP;
        };

        ConstLawType::Type GetConstLawType(void) const
        {
            return ConstLawType::ELASTIC;
        };

        virtual ConstitutiveLaw<doublereal, doublereal> *pCopy(void) const
        {
            ConstitutiveLaw<doublereal, doublereal> *pCL = NULL;

            SAFENEWWITHCONSTRUCTOR(pCL, GsocSatyamClass, GsocSatyamClass(rigid_mod, poisson_ratio));
            return pCL;
        };

        virtual std::ostream &Restart(std::ostream &out) const
        {
            return out << "Rigidity modulus, " << rigid_mod <<" poisson's ratio, "<<poisson_ratio;
        };

        virtual void Update(const doublereal &Eps,const doublereal& /* EpsPrime */  = 0.)
        {
            ConstitutiveLaw<doublereal, doublereal>::Epsilon = Eps;
            ConstitutiveLaw<doublereal, doublereal>::F = 2 * rigid_mod * ( 1 + poisson_ratio ) * ConstitutiveLaw<doublereal, doublereal>::Epsilon ;
        };

};

/* specific functional object(s) */
struct GsocSatyamClassCLR : public ConstitutiveLawRead<doublereal, doublereal>
{
    virtual ConstitutiveLaw<doublereal, doublereal> *
    Read(const DataManager *pDM, MBDynParser &HP, ConstLawType::Type &CLType) {

        ConstitutiveLaw<doublereal, doublereal> *pCL = 0;

        CLType = ConstLawType::ELASTIC;

        if (HP.IsKeyWord("help")) {
			silent_cerr("GsocSatyamModule:\n"
				"        rigmod pois ,\n"
                "        rigidity_modulud, poisson_ratio \n"
				<< std::endl);
			if (!HP.IsArg()) {
				throw NoErr(MBDYN_EXCEPT_ARGS);
			}
		}


        doublereal rig_mod = HP.GetReal();
        if (rig_mod <= 0.)
        {
            silent_cerr("warning, null or negative rigidity modulus "
                        "at line "
                        << HP.GetLineData() << std::endl);
        }

        doublereal poi_rat = HP.GetReal();
        if (poi_rat <= 0.)
        {
            silent_cerr("warning, null or negative poisson's ratio "
                        "at line "
                        << HP.GetLineData() << std::endl);
        }

        SAFENEWWITHCONSTRUCTOR(pCL, GsocSatyamClass, GsocSatyamClass(rig_mod, poi_rat));

        return pCL;
    };
};



extern "C" int
module_init(const char *module_name, void *pdm, void *php)
{
#if 0
	DataManager	*pDM = (DataManager *)pdm;
	MBDynParser	*pHP = (MBDynParser *)php;
#endif

    ConstitutiveLawRead<doublereal, doublereal> *rf1D = new GsocSatyamClassCLR;
    if (!SetCL1D("rigmod" "pois", rf1D))
    {
        delete rf1D;

        silent_cerr("GsocSatyamClass: "
                    "module_init("
                    << module_name << ") "
                                      "failed"
                    << std::endl);

        return -1;
    }

    return 0;
}
