class key
{
private:
 
public:
  key();
  ~key();
  bool bActive = false; // If this key is held
  int iNote;  // Corresponding note to this key
};