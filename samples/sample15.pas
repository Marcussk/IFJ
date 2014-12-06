{ Test interpretu - behova chyba deleni nulou (navratovy kod 8) }
var
  i: integer;
  j: integer;
  k: real;
begin
  i:=42;
  j:=8;
  k:=i/j;
  write(k)
end.
