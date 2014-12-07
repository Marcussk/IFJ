{zretazena inicializacia premennych}
var       
    Num1, Num2, Sum : Integer;

begin {no semicolon}
 write('Input number 1:'); 
 readln(Num1);
 write('Input number 2:');
 readln(Num2);
 Sum := Num1 + Num2; {addition} 
 write(Sum);
 readln;
end.
