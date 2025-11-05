#include "PairProductioncorrection.hh"

PairProductionCorrection::PairProductionCorrection()
{
axis=PWO_100; // PWO_001/PWO_100 (used W111 for axis 001 of PWO!?!) - edited by Mattia
}
void PairProductionCorrection::set_axis(axis_material a)
{
    axis=a;
}

G4double  PairProductionCorrection::ComputeCorrection(G4double eKineticEnergy)
{
    switch (static_cast<int>(axis)) {
    case PWO:
    return ComputeCorrectionPWO(eKineticEnergy);
    case W111:
     return  ComputeCorrectionW_111(eKineticEnergy);
    case PWO_001:
    return ComputeCorrectionPWO_001(eKineticEnergy);
    case PWO_100: // added by Mattia
    return ComputeCorrectionPWO_100(eKineticEnergy); // added by Mattia
    case W111_3mrad:
    return ComputeCorrectionW_111_3mrad(eKineticEnergy);;
    default:
        printf("\nWrong type of axis and material %d.\n", axis);
        return 1;
    }
}

G4double PairProductionCorrection::ComputeCorrectionPWO(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<20.0)
        {
            value=1.0 - 0.01*energyG + 0.001 *energyG*energyG;
        }
        else
        {
            if(energyG<40)
            {
                value=0.72 + 0.02*energyG + 0.0002*energyG*energyG ;
            }
            else
            {
                if(energyG<100)
                {
                    value=0.135 + 0.0476583*energyG - 0.000125*energyG*energyG - 2.08333e-8*energyG*energyG*energyG;
                }
                else
                {
                    if(energyG<300)
                    {
                        value=-2.081 + 0.0864267*energyG - 0.0003403*energyG*energyG + 4.71333e-7*energyG*energyG*energyG;
                    }
                    else
                    {
                        if(energyG<600)
                        {
                            value=4.095 + 0.007835*energyG - 5.55e-6*energyG*energyG;
                        }
                        else
                        {
                            value=4.317 + 0.00592*energyG - 2.975e-6*energyG*energyG;
                        };
                    };
                };

            };
        };
    }
    else
    {
        value=1;
    };
    return value;
}

G4double PairProductionCorrection::ComputeCorrectionPWO_001(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<67.4)
        {
            value=0.763848+0.0151049*energyG + 0.000477248 *energyG*energyG -4.70277e-06 *energyG*energyG*energyG;
        }
        else
        {
            if(energyG<167.8584)
            {
                value=-0.115807  + 0.0588293*energyG -0.000348754*energyG*energyG+7.72463e-07*energyG*energyG*energyG;
            }
            else
            {
                if(energyG<290)
                {
                    value=1.9767 + 0.0143781*energyG -3.42286e-05*energyG*energyG +3.46971e-08*energyG*energyG*energyG;
                }
                else
                {
                    if(energyG<557)
                    {
                        value=3.34553 + 0.00388837*energyG -5.36416e-06*energyG*energyG + 3.36887e-09*energyG*energyG*energyG;
                    }
                    else
                    {
                        if(energyG<758)
                        {
                            value=4.57564 -0.000686347*energyG +7.74603e-07*energyG*energyG;
                        }
                        else
                        {
                            value=4.26434 + 0.000535472*energyG -2.95232e-07*energyG*energyG;
                        }
                    }
                }

            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}

// added by Mattia ///////////////////////////////////////
G4double PairProductionCorrection::ComputeCorrectionPWO_100(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;
	
    energyG=gammaEnergy/GeV;
	
	//if (energyG<10.0){value=0.9589112;}
	//else if ((energyG>=10.0000)&&(energyG<21.5000)){value=((1.1431350-0.8917249)/(21.5000-10.0000))*energyG + (1.1431350-((1.1431350-0.8917249)/(21.5000-10.0000))*21.5000);}
	//else if ((energyG>=21.5000)&&(energyG<34.7250)){value=((1.5287260-1.1431350)/(34.7250-21.5000))*energyG + (1.5287260-((1.5287260-1.1431350)/(34.7250-21.5000))*34.7250);}
	//else if ((energyG>=34.7250)&&(energyG<49.9337)){value=((1.9301360-1.5287260)/(49.9337-34.7250))*energyG + (1.9301360-((1.9301360-1.5287260)/(49.9337-34.7250))*49.9337);}
	//else if ((energyG>=49.9337)&&(energyG<67.4231)){value=((2.2444790-1.9301360)/(67.4231-49.9337))*energyG + (2.2444790-((2.2444790-1.9301360)/(67.4231-49.9337))*67.4231);}
	//else if ((energyG>=67.4231)&&(energyG<87.5373)){value=((2.5758420-2.2444790)/(87.5373-67.4231))*energyG + (2.5758420-((2.5758420-2.2444790)/(87.5373-67.4231))*87.5373);}
	//else if ((energyG>=87.5373)&&(energyG<110.668)){value=((2.8714440-2.5758420)/(110.668-87.5373))*energyG + (2.8714440-((2.8714440-2.5758420)/(110.668-87.5373))*110.668);}
	//else if ((energyG>=110.668)&&(energyG<137.268)){value=((3.0794660-2.8714440)/(137.268-110.668))*energyG + (3.0794660-((3.0794660-2.8714440)/(137.268-110.668))*137.268);}
	//else if ((energyG>=137.268)&&(energyG<167.858)){value=((3.2827040-3.0794660)/(167.858-137.268))*energyG + (3.2827040-((3.2827040-3.0794660)/(167.858-137.268))*167.858);}
	//else if ((energyG>=167.858)&&(energyG<203.032)){value=((3.4481350-3.2827040)/(203.032-167.858))*energyG + (3.4481350-((3.4481350-3.2827040)/(203.032-167.858))*203.032);}
	//else if ((energyG>=203.032)&&(energyG<243.492)){value=((3.5212030-3.4481350)/(243.492-203.032))*energyG + (3.5212030-((3.5212030-3.4481350)/(243.492-203.032))*243.492);}
	//else if ((energyG>=243.492)&&(energyG<290.016)){value=((3.6501060-3.5212030)/(290.016-243.492))*energyG + (3.6501060-((3.6501060-3.5212030)/(290.016-243.492))*290.016);}
	//else if ((energyG>=290.016)&&(energyG<343.519)){value=((3.7848920-3.6501060)/(343.519-290.016))*energyG + (3.7848920-((3.7848920-3.6501060)/(343.519-290.016))*343.519);}
	//else if ((energyG>=343.519)&&(energyG<405.047)){value=((3.8709340-3.7848920)/(405.047-343.519))*energyG + (3.8709340-((3.8709340-3.7848920)/(405.047-343.519))*405.047);}
	//else if ((energyG>=405.047)&&(energyG<475.804)){value=((3.9373080-3.8709340)/(475.804-405.047))*energyG + (3.9373080-((3.9373080-3.8709340)/(475.804-405.047))*475.804);}
	//else if ((energyG>=475.804)&&(energyG<557.174)){value=((3.9766510-3.9373080)/(557.174-475.804))*energyG + (3.9766510-((3.9766510-3.9373080)/(557.174-475.804))*557.174);}
	//else if ((energyG>=557.174)&&(energyG<650.750)){value=((4.0113810-3.9766510)/(650.750-557.174))*energyG + (4.0113810-((4.0113810-3.9766510)/(650.750-557.174))*650.750);}
	//else if ((energyG>=650.750)&&(energyG<758.363)){value=((4.0519050-4.0113810)/(758.363-650.750))*energyG + (4.0519050-((4.0519050-4.0113810)/(758.363-650.750))*758.363);}
	//else if ((energyG>=758.363)&&(energyG<882.117)){value=((4.0899600-4.0519050)/(882.117-758.363))*energyG + (4.0899600-((4.0899600-4.0519050)/(882.117-758.363))*882.117);}
	//else if ((energyG>=882.117)&&(energyG<1024.43)){value=((4.0596730-4.0899600)/(1024.43-882.117))*energyG + (4.0596730-((4.0596730-4.0899600)/(1024.43-882.117))*1024.43);}
	//else {value=4.5030590;}
	
	if (energyG<10.0){value=0.9214057;}
	else if ((energyG>=10.0000)&&(energyG<21.5000)){value=((1.2739110-0.9214057)/(21.5000-10.0000))*energyG + (1.2739110-((1.2739110-0.9214057)/(21.5000-10.0000))*21.5000);}
	else if ((energyG>=21.5000)&&(energyG<34.7250)){value=((1.7406040-1.2739110)/(34.7250-21.5000))*energyG + (1.7406040-((1.7406040-1.2739110)/(34.7250-21.5000))*34.7250);}
	else if ((energyG>=34.7250)&&(energyG<49.9337)){value=((2.2199660-1.7406040)/(49.9337-34.7250))*energyG + (2.2199660-((2.2199660-1.7406040)/(49.9337-34.7250))*49.9337);}
	else if ((energyG>=49.9337)&&(energyG<67.4231)){value=((2.6710520-2.2199660)/(67.4231-49.9337))*energyG + (2.6710520-((2.6710520-2.2199660)/(67.4231-49.9337))*67.4231);}
	else if ((energyG>=67.4231)&&(energyG<87.5373)){value=((3.0351200-2.6710520)/(87.5373-67.4231))*energyG + (3.0351200-((3.0351200-2.6710520)/(87.5373-67.4231))*87.5373);}
	else if ((energyG>=87.5373)&&(energyG<110.668)){value=((3.3095190-3.0351200)/(110.668-87.5373))*energyG + (3.3095190-((3.3095190-3.0351200)/(110.668-87.5373))*110.668);}
	else if ((energyG>=110.668)&&(energyG<137.268)){value=((3.5742970-3.3095190)/(137.268-110.668))*energyG + (3.5742970-((3.5742970-3.3095190)/(137.268-110.668))*137.268);}
	else if ((energyG>=137.268)&&(energyG<167.858)){value=((3.8385530-3.5742970)/(167.858-137.268))*energyG + (3.8385530-((3.8385530-3.5742970)/(167.858-137.268))*167.858);}
	else if ((energyG>=167.858)&&(energyG<203.032)){value=((4.0308920-3.8385530)/(203.032-167.858))*energyG + (4.0308920-((4.0308920-3.8385530)/(203.032-167.858))*203.032);}
	else if ((energyG>=203.032)&&(energyG<243.492)){value=((4.1808250-4.0308920)/(243.492-203.032))*energyG + (4.1808250-((4.1808250-4.0308920)/(243.492-203.032))*243.492);}
	else if ((energyG>=243.492)&&(energyG<290.016)){value=((4.3523120-4.1808250)/(290.016-243.492))*energyG + (4.3523120-((4.3523120-4.1808250)/(290.016-243.492))*290.016);}
	else {value=4.3523120;}
	//
	//else if ((energyG>=290.016)&&(energyG<343.519)){value=((3.7848920-3.6501060)/(343.519-290.016))*energyG + (3.7848920-((3.7848920-3.6501060)/(343.519-290.016))*343.519);}
	//else if ((energyG>=343.519)&&(energyG<405.047)){value=((3.8709340-3.7848920)/(405.047-343.519))*energyG + (3.8709340-((3.8709340-3.7848920)/(405.047-343.519))*405.047);}
	//else if ((energyG>=405.047)&&(energyG<475.804)){value=((3.9373080-3.8709340)/(475.804-405.047))*energyG + (3.9373080-((3.9373080-3.8709340)/(475.804-405.047))*475.804);}
	//else if ((energyG>=475.804)&&(energyG<557.174)){value=((3.9766510-3.9373080)/(557.174-475.804))*energyG + (3.9766510-((3.9766510-3.9373080)/(557.174-475.804))*557.174);}
	//else if ((energyG>=557.174)&&(energyG<650.750)){value=((4.0113810-3.9766510)/(650.750-557.174))*energyG + (4.0113810-((4.0113810-3.9766510)/(650.750-557.174))*650.750);}
	//else if ((energyG>=650.750)&&(energyG<758.363)){value=((4.0519050-4.0113810)/(758.363-650.750))*energyG + (4.0519050-((4.0519050-4.0113810)/(758.363-650.750))*758.363);}
	//else if ((energyG>=758.363)&&(energyG<882.117)){value=((4.0899600-4.0519050)/(882.117-758.363))*energyG + (4.0899600-((4.0899600-4.0519050)/(882.117-758.363))*882.117);}
	//else if ((energyG>=882.117)&&(energyG<1024.43)){value=((4.0596730-4.0899600)/(1024.43-882.117))*energyG + (4.0596730-((4.0596730-4.0899600)/(1024.43-882.117))*1024.43);}
	//else {value=4.0596730;}
	
    return value;
}
//////////////////////////////////////////////////////////

G4double PairProductionCorrection::ComputeCorrectionW_111(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<62)
        {
            value=0.299031+0.0841384*energyG -0.000450991 *energyG*energyG;
        }
        else
        {
            if(energyG<160)
            {
                value=2.03933  + 0.0327963*energyG -7.51477e-05*energyG*energyG;
            }
            else
            {
                    value=4.47771 + 0.00579909*energyG -2.46054e-06*energyG*energyG;
            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}

G4double PairProductionCorrection::ComputeCorrectionW_111_3mrad(G4double gammaEnergy)
{
    //calculate correction for cross section
    //  approximation from 10 to 1 TeV  by  polynomial
    G4double value=1;
    G4double energyG;

    energyG=gammaEnergy/GeV;
    if(energyG>10.0)
    {
        if(energyG<62)
        {
            value=0.732456+0.0672808*energyG -0.000514399 *energyG*energyG;
        }
        else
        {
            if(energyG<160)
            {
                value=2.32235  + 0.0125342*energyG -3.28697e-05*energyG*energyG;
            }
            else
            {
                    value=3.04292 + 0.00363105*energyG -5.30128e-6*energyG*energyG;
            }
        };
    }
    else
    {
        value=1;
    };
    return value;
}
