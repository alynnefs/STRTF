With Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;
Use Ada.Integer_Text_IO, Ada.Text_IO, Ada.Calendar;

procedure trabalho04 is

  X:Integer;
  indice: Integer:= 1;
  i: Integer:= 1;
  flag1: Integer:=1; -- flag para Mostra1
  flag2: Integer:=0; -- flag para Mostra2
  flag3: Integer:=0; -- flag para Mostra
  vetor: array(1..30) of Character;

  task type Mostra1 is
    --entry mostra(Item: in Integer);
  end Mostra1;
  task body Mostra1 is
    begin
      delay 2.0;
      while indice < 29 loop
         while flag1 = 0 loop
            X:=X+1; -- só para não deixar em branco
         end loop;
         vetor(indice):='a';
         indice:=indice+1;
         flag1:=0;
         flag2:=1;
      end loop;
  end Mostra1;

  task type Mostra2 is
    -- entry mostra(Item: in Integer);
  end Mostra2;
  task body Mostra2 is
    begin
      delay 2.0;
      while indice < 30 loop
         while flag2 = 0 loop
            X:=X+1; -- só para não deixar em branco
         end loop;
        vetor(indice):='b';
        indice:=indice+1;
        flag2:=0;
        flag3:=1;
      end loop;

  end Mostra2;

  task type Mostra3 is
  --entry mostra(Item: in Integer);
  end Mostra3;
  task body Mostra3 is
  begin
  delay 2.0;
  while indice < 31 loop
    while flag3 = 0 loop
      X:=X+1; -- só para não deixar em branco
    end loop;
    vetor(indice):='c';
    indice:=indice+1;
    flag3:=0;
    flag1:=1;
  end loop;
  for i in 1..30 loop
    Put(vetor(i));
  end loop;
  end Mostra3;


  A : Mostra1;
  B : Mostra2;
  C : Mostra3;

begin   
  null;

end trabalho04;   
