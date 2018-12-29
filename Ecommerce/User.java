import java.text.SimpleDateFormat;
import java.util.*;

public class User {
	private String name;
	private String password;
	
	User(String name, String password) {
		this.name = name;
		this.password = password;
	}
	
	public String getPassword() {
		return password;
	}
	
	public String getName() {
		return name;
	}
}

class Buyer extends User {
	private Map<String, Seller> sellerMap;
	Map<Product, Integer> cartMap;
	List<Transaction> transactionList;
	
	Buyer(String name, String password, Map<String, Seller> sellerMap) {
		super(name, password);
		this.cartMap = new HashMap<> ();
		this.sellerMap = sellerMap;
		this.transactionList = new ArrayList<> ();
	}
	
	void checkOut() {
		double totalPrice = 0;
		for(Product product : cartMap.keySet()) {
			totalPrice += product.getPrice();
		}
		System.out.println("the total price of your purchase is " + totalPrice);
		buildTransaction(totalPrice);
	}
	
	private void buildTransaction(double totalPrice) {
		// TODO Auto-generated method stub
		SimpleDateFormat df = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		String time = df.format(new Date());
		Transaction transaction = new Transaction(time, totalPrice, getName());
		transactionList.add(transaction);
		System.out.println("transaction is built at " + time);
	}
	
	void browseTransaction() {
		for (Transaction transaction : transactionList) {
			System.out.println(transaction.getDate());
			System.out.println(transaction.getPayment());
			System.out.println(transaction.getBuyerName());
		}
	}

	void addCart(String[] buyerCom, Set<Product> inventorySet) {
		for (Product product : inventorySet){
			if (product.getName().equals(buyerCom[2])) {
				cartMap.put(product, cartMap.getOrDefault(product, 0) + Integer.parseInt(buyerCom[3]));
				System.out.println(product.getName() + " from " + buyerCom[5] + " added to cart");
				for (String sellerName : sellerMap.keySet()) {
					if (sellerName.equals(buyerCom[5])) {
						sellerMap.get(sellerName).sellerInventory.put(product, sellerMap.get(sellerName).sellerInventory.get(product) - Integer.parseInt(buyerCom[3]));
					}
				}
				break;
			} else {
				System.out.println("no inventory!");
				sellerMap.get(buyerCom[5]).backOrder(product, Integer.parseInt(buyerCom[3]));     //back order
			}
		}
	}

	void removeCart(String[] buyerCom, Set<Product> inventorySet) {
		for (Product product : inventorySet){
			if (product.getName().equals(buyerCom[2])) {
				cartMap.put(product, cartMap.getOrDefault(product, 0) - Integer.parseInt(buyerCom[3]));
				System.out.println(product.getName() + " removed from cart");
				for (String sellerName : sellerMap.keySet()) {
					if (sellerName.equals(buyerCom[5])) {
						sellerMap.get(sellerName).sellerInventory.put(product, sellerMap.get(sellerName).sellerInventory.get(product) + Integer.parseInt(buyerCom[3]));
					}
				}
				break;
			} else {
				System.out.println("not in cart!");
			}
		}
	}
	
	void browseCart() {
		for(Product product : cartMap.keySet()) {
			System.out.println(product.getName() + " " + cartMap.get(product));
		}
	}
}

class Seller extends User {
	Map<Product, Integer> sellerInventory;
	Map<String, Promotion> promotionMap; 
	private Map<String, Seller> sellerMap;
	private Map<String, Buyer> buyerMap;
	Seller(String name, String password, Map<String, Seller> sellerMap, Map<String, Buyer> buyerMap) {
		super(name, password);
		this.sellerInventory = new HashMap<> ();
		this.promotionMap = new HashMap<> ();
		this.sellerMap = sellerMap;
		this.buyerMap = buyerMap;
	}

	void backOrder(Product product, int num) {
		// TODO Auto-generated method stub
		for (String sellerName : sellerMap.keySet()) {
			if (sellerMap.get(sellerName).sellerInventory.containsKey(product)) {
				sellerMap.get(sellerName).sellerInventory.put(product, sellerMap.get(sellerName).sellerInventory.get(product) - num) ;
				sellerInventory.put(product, num);
			}
		}
	}

	void addInventory(String[] sellerCom, Set<Product> inventorySet) {
		
		inventorySet.add(new Product(sellerCom[3], sellerCom[4], Double.parseDouble(sellerCom[5]))); 
		for (Product product : inventorySet) {
			if (product.getName().equals(sellerCom[3])) {
				sellerInventory.put(product, sellerInventory.getOrDefault(product, 0) + Integer.parseInt(sellerCom[2]));
				break;
			}
		}	
		System.out.println(sellerCom[2] + " " + sellerCom[3] + " is added");
	}
	
	void browseInventory() {
		for(Product product : sellerInventory.keySet()) {
			System.out.println(product.getName() + " " + sellerInventory.get(product));
		}	
	}
	
	void removeInventory(String[] sellerCom, Set<Product> inventorySet) {
		for (Product product : inventorySet) {
			if (product.getName().equals(sellerCom[3])) {
				sellerInventory.put(product, sellerInventory.getOrDefault(product, 0) - Integer.parseInt(sellerCom[2]));
			}
		}
		System.out.println(sellerCom[2] + " " + sellerCom[3] + " is removed");
	}
	
	void setPromotion(String[] sellerCom) {      //set fixpromotion/ratepromotion productName content
		if (sellerCom[1].equals("fixPromotion")) {
			for (Product product : sellerInventory.keySet()) {
				if (product.getName().equals(sellerCom[2])) {
					product.setPrice(product.getPrice() - Double.parseDouble(sellerCom[3])); //-= Double.parseDouble(sellerCom[3]);
					System.out.println(sellerCom[2] + " reduced price by " + sellerCom[3] + " dollar");
					promotionMap.put(product.getName(), new Promotion(sellerCom[1], sellerCom[3]));
				}
			}
		} else if (sellerCom[1].equals("ratePromotion")) {
			for (Product product : sellerInventory.keySet()) {
				if (product.getName().equals(sellerCom[2])) {
					product.setPrice(product.getPrice() * Double.parseDouble(sellerCom[3]));
					System.out.println(sellerCom[2] + " reduced price to " + sellerCom[3] + " off");
					promotionMap.put(product.getName(), new Promotion(sellerCom[1], sellerCom[3]));
				}
			}
		}
	}
	
	void browsePromotion() {
		for (String productionName : promotionMap.keySet()) {
			System.out.println(productionName + ": " + promotionMap.get(productionName).getDescription());
		}
	}

	public void browseTransaction(String buyerName) {
		// TODO Auto-generated method stub
		for (String name : buyerMap.keySet()) {
			if (name.equals(buyerName)) {
				buyerMap.get(name).browseTransaction();
			}
		}
	}
}
