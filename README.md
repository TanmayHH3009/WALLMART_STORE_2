                                  **Problem for R3 Batch**
                       Walmart Store System BPlus Tree Implementation


Using a tree data structure, design and develop a system for Walmart Store. In Walmart Store, there are different aisles. In one aisle there are dairy products. In the other there are pulses and grains. In the third there are bath and cleaning products. In fourth aisle they have ready to eat food items. In 5th aisle there are vegetables. Maintain an array of pointers to tree nodes. For each aisle, there is a tree. The nodes in the tree are the items in that aisle with information like the item id, item_name, quantity, expiry_date, threshold quantity. Now there is another tree of bills. When a user comes to the Walmart store, he should be billed for whatever items he buys. The corresponding quantity of each item is to be decremented in the main array of tree pointers of items. The array of aisles should be sorted according to the aisle_number. Items are stored by item_id in each tree of the aisle. In short the item id is the key of the node and and , item_name, quantity, expiry_date, threshold quantity is the info at that node.

Implement the following functions :

• Add / update item – Adds or updates an item in the database.

• Delete an item – Delete the item

• Add aisle/update aisle – Adds or updates an aisle

• Delete aisle – Deletes aisle

• Check availability of particular item for particular quantity and within the expiry date. o Inputs – Item_id, quantity required for item_id and the expiry date

• Function to generate a message when any particular item goes beyond its defined threshold quantity, so that it can be planned to order it soon.

• Function to generate a message on a certain ready-to-eat item if its expiry is within one week.

• Function to generate a message when certain dairy product is likely to expire within one day.

• Merge two aisles in a single aisle. For example, Wallmart wishes to add over the counter medicine section to the store. So, they have less space and want to merge their two aisles, namely, dairy and vegetables.

• Write a function that takes item_id as input and provides a list of items which are more often bought with the given item_id.

• Search all items in a particular aisle in the range between two given item_ids and print their information.

Example where this would be useful - You are the owner of Walmart. You want to optimise your business, so you want to do some data mining and find out the patterns in the data. You want to make a strategic decision as to whether people who buy milk also buy bread and people who buy bread also buy eggs and so they want to make a decision as to whether bread, eggs and milk kept together will make it easy for the customers and in turn increase their sales.

File handling is recommended to be used. The input should be scanned from a file to build the database of aisles, and items in each aisle with their data like quantity, expiry date, threshold quantity etc.
