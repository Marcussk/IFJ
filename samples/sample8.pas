{Test na cislicove literaly}
var
	a : integer;
	b : integer;
	c : integer;
	d : integer;
	vysl : integer;
begin
	a := 0010;
	write('Hodnota a je: ', a, ''#10'');
	b := 000010.00004;
	write('Hodnota b je: ', b, ''#10'');
	c := 0010e-4;
	write('Hodnota c je: ', c, ''#10'');	
	d := 0010e+4;
	write('Hodnota d je: ', d, ''#10'');
end.
