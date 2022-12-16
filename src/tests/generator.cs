using System.IO;
using System;
class Program
{
  public static void Main()
  {
    s21_add();
    s21_sub();
    s21_mul();
    s21_div();
    s21_mod();

    s21_is_less();
    s21_is_less_or_equal();
    s21_is_greater();
    s21_is_greater_or_equal();
    s21_is_equal();
    s21_is_not_equal();

    s21_from_int_to_decimal();
    s21_from_float_to_decimal();
    s21_from_decimal_to_int();
    s21_from_decimal_to_float();

    s21_floor();
    s21_round();
    s21_negate();
    s21_truncate();
  }
  public static void s21_add()
  {
    string path = "s21_add.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        for (decimal j = -3.1415926535897932384626433833m; j < 3.1415926535897932384626433833m; j += 3.1415926535897932384626433833m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i + j);
        }
      }
      for (decimal i = -314159.26535897932384626433833m; i < 314159.26535897932384626433833m; i += 314159.26535897932384626433833m / 20m)
      {
        for (decimal j = -314.15926535897932384626433833m; j < 314.15926535897932384626433833m; j += 314.15926535897932384626433833m / 20m)
        {
          if ((i != 298.45130209103035765395112150m) && (j != 298.45130209103035765395112150m))
          {
            writer.Write(i);
            writer.Write(j);
            writer.Write(i + j);
          }
        }
      }
    }
  }
  public static void s21_sub()
  {
    string path = "s21_sub.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        for (decimal j = -3.1415926535897932384626433833m; j < 3.1415926535897932384626433833m; j += 3.1415926535897932384626433833m / 20m)
        {

          writer.Write(i);
          writer.Write(j);
          writer.Write(i - j);
        }
      }
      for (decimal i = -314159.26535897932384626433833m; i < 314159.26535897932384626433833m; i += 314159.26535897932384626433833m / 20m)
      {
        for (decimal j = -314.15926535897932384626433833m; j < 314.15926535897932384626433833m; j += 314.15926535897932384626433833m / 20m)
        {
          if ((i != 298.45130209103035765395112150m) && (j != 298.45130209103035765395112150m))
          {
            writer.Write(i);
            writer.Write(j);
            writer.Write(i - j);
          }
        }
      }
      for (decimal i = -314159.26535897932384626433833m; i < 314159.26535897932384626433833m; i += 314159.26535897932384626433833m / 20m)
      {
        for (decimal j = -314.15926535897932384626433833m; j < 314.15926535897932384626433833m; j += 314.15926535897932384626433833m / 20m)
        {
          if ((i != 298.45130209103035765395112150m) && (j != 298.45130209103035765395112150m))
          {
            writer.Write(i);
            writer.Write(j);
            writer.Write(j - i);
          }
        }
      }
    }
  }
  public static void s21_mul()
  {
    string path = "s21_mul.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        for (decimal j = -3.1415926535897932384626433833m; j < 3.1415926535897932384626433833m; j += 3.1415926535897932384626433833m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i * j);
        }
      }
      writer.Write(1m);
      writer.Write(1m);
      writer.Write(1m);
      for (decimal i = -100m; i < 100m; i += 100m / 20m)
      {
        for (decimal j = -100m; j < 100m; j += 100m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i * j);
        }
      }
      for (decimal i = -314159.26535897932384626433833m; i < 314159.26535897932384626433833m; i += 314159.26535897932384626433833m / 20m)
      {
        for (decimal j = -314.15926535897932384626433833m; j < 314.15926535897932384626433833m; j += 314.15926535897932384626433833m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i * j);
        }
      }

    }

  }
  public static void s21_div()
  {
    string path = "s21_div.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        for (decimal j = -3.1415926535897932384626433833m; j < 3.1415926535897932384626433833m; j += 3.1415926535897932384626433833m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i / j);
        }
      }
      for (decimal i = 1m; i < 100m; i += 100m / 20m)
      {
        for (decimal j = 1m; j < 100m; j += 100m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i / j);
        }
      }
    }
  }
  public static void s21_mod()
  {
    string path = "s21_mod.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        for (decimal j = -3.1415926535897932384626433833m; j < 3.1415926535897932384626433833m; j += 3.1415926535897932384626433833m / 20m)
        {
          writer.Write(i);
          writer.Write(j);
          writer.Write(i % j);
        }
      }
    }
  }
  public static void s21_is_less()
  {
    string path = "s21_is_less.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9889876545667765m;
      j = -87677880m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = 1112345m;
      j = -1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i < j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }
  public static void s21_is_less_or_equal()
  {
    string path = "s21_is_less_or_equal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {

      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9889876545667765m;
      j = -87677880m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = 1112345m;
      j = -1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i <= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }
  public static void s21_is_greater()
  {
    string path = "s21_is_greater.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9889876545667765m;
      j = -87677880m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112345m;
      j = 1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i > j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }
  public static void s21_is_greater_or_equal()
  {
    string path = "s21_is_greater_or_equal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9889876545667765m;
      j = -87677880m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112345m;
      j = 1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i >= j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }
  public static void s21_is_equal()
  {
    string path = "s21_is_equal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      // i = -9889876545667765m; // !
      // j = -87677880m;
      i = -1m; // !
      j = -2m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = 1112345m;
      j = -1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i == j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }
  public static void s21_is_not_equal()
  {
    string path = "s21_is_not_equal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      decimal i;
      decimal j;

      i = 0m;
      j = 0m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 0m;
      j = -0m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 9876m;
      j = -990m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = -9889876545667765m; // !
      j = -87677880m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }
      i = 556.00000000m;
      j = 556.0000m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = -1112343m;
      j = -1112343m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

      i = 1112345m;
      j = -1112345m;
      writer.Write(i);
      writer.Write(j);
      if (i != j)
      {
        writer.Write((int)1);
      }
      else { writer.Write((int)0); }

    }
  }




  public static void s21_floor()
  {
    string path = "s21_floor.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        writer.Write(i);
        writer.Write(decimal.Floor(i));
      }
    }
  }
  public static void s21_round()
  {
    string path = "s21_round.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        writer.Write(i);
        writer.Write(decimal.Round(i));
      }
    }
  }
  public static void s21_negate()
  {
    string path = "s21_negate.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 0.31415926535897932384626433833m)
      {
        writer.Write(i);
        writer.Write(decimal.Negate(i));
      }
    }
  }
  public static void s21_truncate()
  {
    string path = "s21_truncate.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        writer.Write(i);
        writer.Write(decimal.Truncate(i));
      }
    }
  }
  public static void s21_from_int_to_decimal()
  {
    string path = "s21_from_int_to_decimal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (int i = -100; i < 100; i++)
      {
        writer.Write(i);
        writer.Write((decimal)i);
      }
      for (int i = -12000000; i < -12000100; i++)
      {
        writer.Write(i);
        writer.Write((decimal)i);
      }
      writer.Write(Int32.MinValue);
      writer.Write((decimal)Int32.MinValue);
      writer.Write(Int32.MaxValue);
      writer.Write((decimal)Int32.MaxValue);
    }
  }

  public static void s21_from_decimal_to_int()
  {
    string path = "s21_from_decimal_to_int.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -100m; i < 100m; i++)
      {
        writer.Write(i);
        writer.Write((int)i);
      }
      for (decimal i = -100000m; i < 10000m; i += 1000m)
      {
        writer.Write(i);
        writer.Write((int)i);
      }
      for (decimal i = -3.1415926535897932384626433833m; i < 3.1415926535897932384626433833m; i += 3.1415926535897932384626433833m / 20m)
      {
        writer.Write(i);
        writer.Write(decimal.ToInt32(i));
      }
      writer.Write(decimal.ToInt32(Int32.MinValue));
      writer.Write(Int32.MinValue);
      writer.Write(decimal.ToInt32(Int32.MaxValue));
      writer.Write(Int32.MaxValue);

    }
  }

  public static void s21_from_float_to_decimal()
  {
    string path = "s21_from_float_to_decimal.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (float i = -100; i < 100; i += 1f)
      {
        writer.Write(i);
        writer.Write(((decimal)i));
      }
      for (float i = -1000000; i < 1000000; i += 1000f)
      {
        writer.Write(i);
        writer.Write(((decimal)i));
      }
      for (float i = -3.141f * 100f; i < 3.141f * 100f; i += 3.1415f * 100f / 20f)
      {
        writer.Write(i);
        writer.Write(((decimal)i));
      }
    }
  }
  public static void s21_from_decimal_to_float()
  {
    string path = "s21_from_decimal_to_float.input";
    using (BinaryWriter writer = new BinaryWriter(File.Open(path, FileMode.OpenOrCreate)))
    {
      for (decimal i = -100m; i < 100m; i += 1m)
      {
        writer.Write(i);
        writer.Write((float)i);
      }
      for (decimal i = -1000000m; i < 1000000m; i += 1000m)
      {
        writer.Write(i);
        writer.Write((float)i);
      }
      for (decimal i = -3.1415926535897932384626433833m * 100m; i < -3.1415926535897932384626433833m * 100m; i += -3.1415926535897932384626433833m * 100m / 20m)
      {
        writer.Write(i);
        writer.Write((float)i);
      }
    }
  }
}



