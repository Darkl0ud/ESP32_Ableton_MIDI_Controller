class key
{
private:
 
public:
  key();
  ~key();
  bool bActive = false; // If this key is active
  int iNote;  // Corresponding note to this key
  int noteMode;  // 0 = One Shot, 1 = Toggle
};