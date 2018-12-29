import java.io.File;
import java.util.*;
import java.io.*;

class Parser {
	private Map<String, Buyer> buyerMap;
	private Map<String, Seller> sellerMap;
	private Set<Product> inventorySet;
	
	Parser(Map<String, Buyer> buyerMap, Map<String, Seller> sellerMap, Set<Product> inventorySet) throws FileNotFoundException{
		this.buyerMap = buyerMap;
		this.sellerMap = sellerMap;
		this.inventorySet = inventorySet;
	}
	File file = new File("/Users/junmingzhao/Desktop/EcommerceTest.txt");
	Scanner in = new Scanner(file);
	boolean hasNextLine() {
		while(in.hasNextLine()) {
			return true;
		}
		return false;
	}
	
	void nextLine() {
		System.out.println("New User?");
		String str = in.nextLine();
		System.out.println(str);
		start(str);
	}
	
	void start(String str) { //whether a new user
		if (str.equals("yes")) {
			addUser();
		} else if (str.equals("no")) {
			login();
		}
	}
	
	private void login() {
		System.out.println("buyer/seller?");
		String str = in.nextLine();
		System.out.println(str);
		//buyer
		if (str.equals("buyer")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			//If user name and password match
			if (buyerMap.containsKey(loginInfo[0]) && buyerMap.get(loginInfo[0]).getPassword().equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String buyerCommand = in.nextLine();
					String[] buyerCom = buyerCommand.split(" ");
					if (buyerCom[0].equals("add") && buyerCom[1].equals("cart")) { //add cart product num from sellerName
						buyerMap.get(loginInfo[0]).addCart(buyerCom, inventorySet);
					} else if (buyerCom[0].equals("remove") && buyerCom[1].equals("cart")) { //remove cart product num from sellerName
						buyerMap.get(loginInfo[0]).removeCart(buyerCom, inventorySet);
					} else if (buyerCom[0].equals("browse") && buyerCom[1].equals("cart")) { //browse cart
						buyerMap.get(loginInfo[0]).browseCart();
					} else if (buyerCom[0].equals("checkout")) {                   //checkout
						buyerMap.get(loginInfo[0]).checkOut();
					} else if (buyerCom[0].equals("browse") && buyerCom[1].equals("transaction")) { //browse transaction
						buyerMap.get(loginInfo[0]).browseTransaction();
					} else if (buyerCom[0].equals("signout")) { //signout
						break;
					}
				} 
			} else {
				System.out.println("username or password is wrong!");
			}
		} else if (str.equals("seller")) {
			System.out.println("Input username,password");
			str = in.nextLine();
			String[] loginInfo = str.split(",");
			//If user name and password match
			if (sellerMap.containsKey(loginInfo[0]) && sellerMap.get(loginInfo[0]).getPassword().equals(loginInfo[1])) {
				System.out.println("Login successfully");
				while (hasNextLine()) {
					System.out.println("Input your command");
					String sellerCommand = in.nextLine();
					String[] sellerCom = sellerCommand.split(" ");
					if (sellerCom[0].equals("add") && sellerCom[1].equals("inventory")) { //add inventory num productName id price
						sellerMap.get(loginInfo[0]).addInventory(sellerCom, inventorySet);
					} else if (sellerCom[0].equals("browse") && sellerCom[1].equals("inventory")) { //browse inventory
						sellerMap.get(loginInfo[0]).browseInventory();
					} else if (sellerCom[0].equals("remove") && sellerCom[1].equals("inventory")) { //remove inventory num productName
						sellerMap.get(loginInfo[0]).removeInventory(sellerCom, inventorySet);
					} else if (sellerCom[0].equals("set")) {                //set fixpromotion/ratepromotion productName content
						sellerMap.get(loginInfo[0]).setPromotion(sellerCom);
					} else if (sellerCom[0].equals("browse") && sellerCom[1].equals("promotion")) { //browse promotion
						sellerMap.get(loginInfo[0]).browsePromotion();
					} else if (sellerCom[0].equals("browse") && sellerCom[1].equals("transaction")) { //browse transaction of buyerName
						sellerMap.get(loginInfo[0]).browseTransaction(sellerCom[3]);
					} else if (sellerCom[0].equals("signout")) {
						break;
					}
				}
			}
		}	
	}
	
	private void addUser() {
		System.out.println("buyer/seller?");
		String str = in.nextLine();
		System.out.println(str);
		if (str.equals("buyer")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!buyerMap.containsKey(signupInfo[0])) {
				buyerMap.put(signupInfo[0], new Buyer(signupInfo[0], signupInfo[1], sellerMap));//userName password
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		} else if (str.equals("seller")) {
			System.out.println("Create userName & password");
			System.out.println(str = in.nextLine());
			String[] signupInfo = str.split(",");
			if (!sellerMap.containsKey(signupInfo[0])) {
				sellerMap.put(signupInfo[0], new Seller(signupInfo[0], signupInfo[1], sellerMap, buyerMap));
				System.out.println("account " + signupInfo[0] + " created");
			} else 
				System.out.println("Username occupied!");
		}
	}	
}
