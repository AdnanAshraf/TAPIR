//
//  TapirMotherOfAllFilters.m
//  TapirLib
//
//  Created by dilu on 11/26/13.
//  Copyright (c) 2013 Jimin Jeon. All rights reserved.
//

#import "TapirMotherOfAllFilters.h"
#import <Accelerate/Accelerate.h>

@implementation TapirMotherOfAllFilters
-(id)initWithLength:(int)l{
    if(self=[super init]){
        length = l;
        buffer = malloc(sizeof(float)*l);
        coefficients = malloc(sizeof(float)*l);
        for(int i = 0; i<l; i++){
            buffer[i]=0;
            coefficients[i]=0;
        }
    }
    return self;
}

+(id)createHPF1{
    TapirMotherOfAllFilters* filter = [[TapirMotherOfAllFilters alloc] initWithLength:51];
    float coef[] = {-0.000974904910325978405274960358894986712,
    0.001061570369449390963470514215316597983,
    -0.000845968749311988327428679657060683894,
    0.000207446002690359727286401048118591461,
    0.000898872800870446886888076587496243519,
    -0.002280760783206247418325451903342582227,
    0.003421613224756182413538452280477031309,
    -0.003573513116002676164723039775594770617,
    0.002059077967269363866609221958015041309,
    0.001306637501988568150768088571567204781,
    -0.005863888601476439227377923657513747457,
    0.010042861492369518569556241516238515032,
    -0.01172573863896613291513126853260473581,
    0.009010891522838104447656881745842838427,
    -0.001159090966887983296593200854829319724,
    -0.01063785830046301041085232697014362202,
    0.023036860284334149256979529241107229609,
    -0.031097860730212163044017259494466998149,
    0.029574426226330587097468338697581202723,
    -0.014609057854509839569079332477485877462,
    -0.014705576307981381434242251771138398908,
    0.055467322703750356061025428289212868549,
    -0.101195096771377376909661904846871038899,
    0.143197100713312597264348369208164513111,
    -0.172738244745205077324001763372507411987,
    0.183375122580046623355087831441778689623,
    -0.172738244745205077324001763372507411987,
    0.143197100713312597264348369208164513111,
    -0.101195096771377376909661904846871038899,
    0.055467322703750356061025428289212868549,
    -0.014705576307981381434242251771138398908,
    -0.014609057854509839569079332477485877462,
    0.029574426226330587097468338697581202723,
    -0.031097860730212163044017259494466998149,
    0.023036860284334149256979529241107229609,
    -0.01063785830046301041085232697014362202,
    -0.001159090966887983296593200854829319724,
    0.009010891522838104447656881745842838427,
    -0.01172573863896613291513126853260473581,
    0.010042861492369518569556241516238515032,
    -0.005863888601476439227377923657513747457,
    0.001306637501988568150768088571567204781,
    0.002059077967269363866609221958015041309,
    -0.003573513116002676164723039775594770617,
    0.003421613224756182413538452280477031309,
    -0.002280760783206247418325451903342582227,
    0.000898872800870446886888076587496243519,
    0.000207446002690359727286401048118591461,
    -0.000845968749311988327428679657060683894,
    0.001061570369449390963470514215316597983,
        -0.000974904910325978405274960358894986712};
    [filter setCoef:coef length:51];
    return filter;
}

-(void)setCoef:(float *)values length:(int)l{
    for(int i = 0; i<l; i++){
        coefficients[i] = values[i];
    }
}
-(void)next:(float)newValue writeTo:(float *)destination{
    for(int i = 1; i<length; i++){
        buffer[length-i]=buffer[length-i-1];
    }
    buffer[0]=newValue;

    vDSP_dotpr(buffer,1,coefficients,1,destination,length);
}

@end
