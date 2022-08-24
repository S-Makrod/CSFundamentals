#include "graffit.c"

/** 
 * Here's some basic testing code. It's primarily intended to give you an idea of
 * how these functions are used.
 *
 * As always, you should add more tests.
 */
void print_users(){
  int i = 0;
  FriendNode * users = allUsers;
  while(users != NULL){
    printf("%d: %s\n", i, users->user->name);
    users = users->next;
    i++;
  }
  printf("\n");
}

void print_matrix (){
  for(int i = 0; i < MAT_SIZE; i++){
    for(int j = 0; j < MAT_SIZE; j++){
      printf("%d\t", brand_adjacency_matrix[i][j]);
    }
    printf("\n");
  }
  printf("\n");
}

int main() { 
  // Users & Brands
  //GIVEN VERSION
  /*
  User* angela = create_user("Angela");
  User* brian = create_user("Brian");
  add_friend(angela, brian);
  // Expect to see Angela with her friend Brian
  
  print_user_data(angela); 
  
  // Expect to see Brian with his friend Angela
  
  print_user_data(brian); 
  int angela_and_brian = get_degrees_of_connection(angela, brian);
  // Expect 1
  populate_brand_matrix("brands.txt");
  printf("Connections between Angela and Brian: %d\n", angela_and_brian);
  follow_brand(angela, "brandonRufino"); 
  // Angela should now follow brandonRufino.
  // Brandon was a TA for this course and there's a pun too good to give up.
  
  print_user_data(angela); 
  print_brand_data("brandZero"); // Similar to brandonRufino
  print_brand_data("brandonRufino"); // Similar to brandZero
  print_brand_data("brandTwo"); // Not similar to anything
  connect_similar_brands("brandZero", "brandTwo");
  
  print_brand_data("brandZero"); // Similar to brandonRufino and brandTwo
  print_brand_data("brandonRufino"); // Similar to brandZero
  print_brand_data("brandTwo"); // Simlar to brandZero
  
  User* will = create_user("Will");
  add_friend(angela, will);
  print_user_data(will); // Expect to see Will with his friend Angela
  int mutuals_brian_will = get_mutual_friends(brian, will);
  // Expect 1
  printf("Value: %d\n", mutuals_brian_will);

  // Last Part
  // This should return Will
  User* brianSuggested = get_suggested_friend(brian);
  
  print_user_data(brianSuggested); // We should see Will here again
  User* mustafa = create_user("Mustafa");
  add_friend(angela, mustafa);
  add_suggested_friends(mustafa, 2);
  // Expect to see Mustafa with friends Angela, Brian, and Will
  print_user_data(mustafa);
  follow_brand(will, "brandTwo");
  print_user_data(will);
  follow_brand(mustafa, "brandZero"); 
  print_user_data(mustafa);
  follow_suggested_brands(mustafa, 1);
  print_user_data(mustafa); // Mustafa should now also follow brandTwo
  */

  //MY VERSION
  User* angela = create_user("Angela");
  User* brian = create_user("brian");
  User* saad = create_user("Saad");
  User* bob = create_user("Bob");
  User* Brian = create_user("Brian");
  User* Hangela = create_user("Hangela");
  User* jeffery = create_user("Jeffery");
  User* tommy = create_user("Tommy");

  print_users();
  add_friend(saad, Hangela);
  add_friend(brian, angela);
  add_friend(saad, tommy);
  add_friend(Brian, jeffery);
  add_friend(saad, brian);
  add_friend(tommy, Brian);
  add_friend(saad, angela);
  add_friend(angela, Hangela);

  strcpy(brand_names[0], "Nike");
  strcpy(brand_names[1], "Rolex");
  strcpy(brand_names[2], "Bugatti");

  for(int i = 0; i < MAT_SIZE; i++){
    for(int j = 0; j < MAT_SIZE; j++){
      brand_adjacency_matrix[i][j] = 0;
    }
  }
  
  int x = remove_friend(saad, tommy);
  int y = remove_friend(bob, angela);
  printf("x:%d\ny:%d\n",x,y);

  print_user_data(saad);
  printf("================================\n");
  print_user_data(brian);
  printf("================================\n");
  print_user_data(Brian);
  printf("================================\n");

  print_matrix();
  connect_similar_brands("Nike", "Rolex");
  print_matrix();
  connect_similar_brands("Nike", "Bugatti");
  print_matrix();
  connect_similar_brands("Bugatti", "Rolex");
  print_matrix();
  remove_similar_brands("Nike", "Bugatti");
  remove_similar_brands("Nike", "Rolex");
  print_matrix();

  printf("\nName of suggested friend: %s\n", get_suggested_friend(bob)->name);
  int n = add_suggested_friends(bob, 3);
  printf("n: %d\n\n", n);

  follow_brand(bob, "Nike");
  follow_brand(bob, "Bugatti");
  follow_brand(bob, "Rolex");
  follow_brand(saad, "Nike");
  follow_brand(angela, "Bugatti");
  follow_brand(angela, "Rolex");
  follow_brand(jeffery, "Rolex");
  follow_brand(Hangela, "Bugatti");

  follow_brand(bob, "Rolex");
  unfollow_brand(saad, "Nike");
  unfollow_brand(bob, "Nike");
  unfollow_brand(bob, "Rolex");
  unfollow_brand(bob, "Bugatti");

  print_user_data(bob);
  printf("================================\n");

  int z = get_mutual_friends(saad, angela);
  int s = get_mutual_friends(saad, bob);
  int t = get_mutual_friends(jeffery, angela);
  int r = get_mutual_friends(angela, tommy);

  printf("z: %d\ns: %d\nt: %d\nr: %d\n", z, s, t, r);

  int e = get_same_brands(saad, bob);
  int w = get_same_brands(saad, jeffery);
  int g = get_same_brands(saad, Hangela);
  int q = get_same_brands(angela, bob);
  int p = get_same_brands(bob, brian);

  printf("e: %d\nw: %d\nq: %d\np: %d\ng: %d\n", e, w, q, p, g);

  printf("Name of suggested friend: %s\n\n", get_suggested_friend(bob)->name);

  int u = add_suggested_friends(bob, 5);
  print_user_data(bob);
  printf("================================\n");
  printf("u: %d\n", u);

  int c = get_similar_brand_count(bob, "Rolex");
  int v = get_similar_brand_count(saad, "Bugatti");
  int b = get_similar_brand_count(saad, "Rolex");

  printf("c: %d\nv: %d\nb: %d\n", c, v, b);

  if (get_suggested_brands(jeffery) == NULL) printf("NULL\n");
  else printf("%s\n", get_suggested_brands(jeffery));
  if (get_suggested_brands(saad) == NULL) printf("NULL\n");
  else printf("%s\n", get_suggested_brands(saad));
  if (get_suggested_brands(angela) == NULL) printf("NULL\n");
  else printf("%s\n", get_suggested_brands(angela));
  if (get_suggested_brands(brian) == NULL) printf("NULL\n");
  else printf("%s\n", get_suggested_brands(brian));
  if (get_suggested_brands(bob) == NULL) printf("NULL\n");
  else printf("%s\n", get_suggested_brands(bob));

  int f = follow_suggested_brands(saad, 4);
  int o = follow_suggested_brands(brian, 3);
  int l = follow_suggested_brands(angela, 1);
  print_user_data(saad);
  printf("================================\n");
  print_user_data(brian);
  printf("================================\n");
  print_user_data(angela);
  printf("================================\n");
  printf("f: %d\no: %d\nl: %d\n", f, o, l);

  remove_friend(saad, bob);
  printf("Degree of connections: %d\n", get_degrees_of_connection(saad, brian));
  printf("Degree of connections: %d\n", get_degrees_of_connection(saad, jeffery));
  printf("Degree of connections: %d\n", get_degrees_of_connection(saad, bob));
  remove_friend(jeffery, bob);
  printf("Degree of connections: %d\n", get_degrees_of_connection(jeffery, bob));
  remove_friend(saad, tommy);
  printf("Degree of connections: %d\n", get_degrees_of_connection(Brian, brian));
  printf("Degree of connections: %d\n", get_degrees_of_connection(saad, saad));

  return 0;
}
