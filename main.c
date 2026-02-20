#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <float.h>

// (Tensörün o an ne tuttuðunu bilmek için)
typedef enum {
    TIP_FLOAT32,
    TIP_FLOAT16,
    TIP_INT8
} VeriTipi;

// 2. UNION
typedef union {
    float f32;       
    uint16_t f16;    
    int8_t i8;       
} TensorVerisi;

// 3. TENSÖR YAPISI
typedef struct {
    int satir;
    int sutun;
    VeriTipi tip;
    TensorVerisi *veri; 
} Tensor;

// --- TENSÖR OLUÞTURMA  ---
Tensor* tensor_olustur(int satir, int sutun, VeriTipi tip) {
    Tensor* t = (Tensor*)malloc(sizeof(Tensor));
    t->satir = satir;
    t->sutun = sutun;
    t->tip = tip;
    
    t->veri = (TensorVerisi*)calloc(satir * sutun, sizeof(TensorVerisi)); 
    return t;
}

// --- BELLEK TEMÝZLEME  ---
void tensor_serbest_birak(Tensor* t) {
    if (t != NULL) {
        free(t->veri); 
        free(t);       
    }
}

// --- QUANTIZATION ---
void tensor_kuantize_et(Tensor* kaynak_float, Tensor* hedef_int8) {
    int toplam_eleman = kaynak_float->satir * kaynak_float->sutun;
    float min_deger = FLT_MAX;
    float max_deger = -FLT_MAX;
    float olcek;
    int i; 
    
    // Max ve Min deðerleri bul
    for(i = 0; i < toplam_eleman; i++) {
        if(kaynak_float->veri[i].f32 < min_deger) min_deger = kaynak_float->veri[i].f32;
        if(kaynak_float->veri[i].f32 > max_deger) max_deger = kaynak_float->veri[i].f32;
    }

    // Ölçek (Scale) hesapla
    olcek = (max_deger - min_deger) / 255.0f;
    if (olcek == 0.0f) olcek = 1.0f; 
    
    // Tüm float deðerleri Int8'e dönüþtür
    for(i = 0; i < toplam_eleman; i++) {
        float normallestirilmis = (kaynak_float->veri[i].f32 - min_deger) / olcek;
        hedef_int8->veri[i].i8 = (int8_t)(round(normallestirilmis) - 128);
    }
}

// --- (TEST VE DEMO) ---
int main() {
    int satir = 3;
    int sutun = 3;
    int toplam_eleman = satir * sutun;
    int i;
    float ornek_veriler[] = {0.15f, -1.2f, 3.14f, 0.0f, 2.5f, -0.8f, 1.1f, -2.5f, 0.5f};
    
    Tensor* gercek_agirliklar;
    Tensor* sikistirilmis_agirliklar;

    printf("--- TinyML Tensor Yonetimi Basliyor ---\n\n");

    // 1. Orijinal Float32 Tensörü Oluþtur ve Doldur
    gercek_agirliklar = tensor_olustur(satir, sutun, TIP_FLOAT32);
    
    printf("Orijinal Float32 Matrisi (Bellek Tuketimi: %lu Byte):\n", (unsigned long)(toplam_eleman * sizeof(float)));
    for(i = 0; i < toplam_eleman; i++) {
        gercek_agirliklar->veri[i].f32 = ornek_veriler[i];
        printf("%6.2f ", gercek_agirliklar->veri[i].f32);
        if((i + 1) % sutun == 0) printf("\n"); 
    }
    printf("\n");

    // 2. 
    sikistirilmis_agirliklar = tensor_olustur(satir, sutun, TIP_INT8);
    
    // 3. Quantization Ýþlemini Uygula
    tensor_kuantize_et(gercek_agirliklar, sikistirilmis_agirliklar);
    
    printf("Quantize Edilmis Int8 Matrisi (Bellek Tuketimi: %lu Byte):\n", (unsigned long)(toplam_eleman * sizeof(int8_t)));
    for(i = 0; i < toplam_eleman; i++) {
        printf("%6d ", sikistirilmis_agirliklar->veri[i].i8);
        if((i + 1) % sutun == 0) printf("\n");
    }
    printf("\n");

    // RAM'i Temizle 
    tensor_serbest_birak(gercek_agirliklar);
    tensor_serbest_birak(sikistirilmis_agirliklar);
    
    printf("Bellek basariyla temizlendi (No Memory Leak!). Program sonlandiriliyor.\n");
	
	getchar();
    return 0;
}

