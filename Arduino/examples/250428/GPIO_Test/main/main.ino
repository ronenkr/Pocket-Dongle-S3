
#define s 15
//定义LED管脚
const int led_pin[s]={1,2,3,4,5,6,7,8,9,40,41,42,43,44,45};
char a,b,i;
void setup() 
{                
for(i=0;i<=s-1;i++)
  pinMode(led_pin[i], OUTPUT);
}

void loop() 
{
  for(a=0;a<=s-1;a++)
  {
  digitalWrite(led_pin[a], HIGH);  
  delay(500);
  digitalWrite(led_pin[a], LOW);  
  delay(500);
  }
  // 流回来，则不需要两边点亮
  for(b=s-2;b>=1;b--)
  {
  digitalWrite(led_pin[b], HIGH); 
  delay(500);
  digitalWrite(led_pin[b], LOW);  
  delay(500);
  }
}

