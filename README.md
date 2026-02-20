1. GitHub README.md Dosyası (10 Puan)
Bu metni, hocanın istediği "GPT'den çıkmış gibi ve formatı bozuk olmasın" kuralına uyarak tam bir öğrenci sadeliğinde ve teknik doğrulukta hazırladım. Doğrudan kopyalayıp GitHub repona yapıştırabilirsin.

Gömülü Sistemler (TinyML) İçin Dinamik Tensör ve Quantization
Bu proje, RAM kapasitesi oldukça kısıtlı olan mikrodenetleyicilerde (Arduino, ESP32 vb.) yapay zeka modellerini (TinyML) çalıştırabilmek için geliştirilmiş, bellek dostu bir dinamik Tensör (Tensor) mimarisidir.

Projenin Amacı ve Problem
Yapay zeka modellerinin ağırlıkları standart olarak 32-bit Float formatında tutulur. Ancak bu durum, kısıtlı belleğe sahip gömülü cihazların anında çökmesine sebep olur. Bu projede, bellek tüketimini minimize etmek için C dilinin alt seviye yetenekleri kullanılarak özel bir veri yapısı tasarlanmıştır.

Union Mimarisi ve Tip Dönüşümü
Projenin temelinde C dilindeki union yapısı yatmaktadır. TensorVerisi adındaki union sayesinde, aynı bellek adresini duruma göre 32-bit Float, 16-bit Float (Half-precision) veya 8-bit Integer olarak okuyabiliyoruz.
Eğer klasik struct kullansaydık bu üç veri tipi bellekte ayrı ayrı yer kaplayacaktı. Ancak union kullanarak sadece en büyük eleman (4 Byte) kadar yer kaplamasını sağladık ve bellekten büyük oranda tasarruf ettik.

Quantization (Nicemleme)
