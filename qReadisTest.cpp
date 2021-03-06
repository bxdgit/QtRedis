#include "qReadisTest.h"

qReadisTest::qReadisTest(QObject *parent) : QObject(parent)
{}

qReadisTest::~qReadisTest()
{
    delete redis;
}

void qReadisTest::slotMessage(qRedis::Reply reply)
{
    QTime time;
    qDebug() << time.currentTime();
    qDebug() << "Channel:" << reply.channel << "Pattern:" << reply.pattern;
    qDebug() << reply.value.toString();
}

void qReadisTest::startTest()
{
    redis = new qRedis("localhost",6379);
    connect(redis, SIGNAL(returnData(qRedis::Reply)), this, SLOT(slotMessage(qRedis::Reply)));

    qRedis::Reply reply;

    if (!redis->openConnection())
    {
        qDebug() << "Could not connect to server...";
        exit(0);
    }

    qDebug() << "Connected to server...";

    //qDebug() << "AUTH:" << redis->auth("redisZzZ");
    QString values = " {\"id:5411113430101\",\" filepath\":\"/home/bxd/0000.jpg\",\" externsion\":\"jpg\"}";
    qDebug()<<"LPUSH"<<redis->lpush("bxd",values);

    // Set and Get example
    qDebug() << "SET:" << redis->set("key", "\"Hello World\"");
    qDebug() << "GET:" << redis->get("key");

    // Append to Key example
    qDebug() << "SET:" << redis->set("key", "\"Hello\"");
    qDebug() << "EXISTS:" << redis->exists("key");
    qDebug() << "GET:" << redis->get("key");
    qDebug() << "APPEND:" << redis->append("key", "\" World\"");
    qDebug() << "GET:" << redis->get("key");

    // Multi Set and Get example
    QMap<QString,QVariant> keypairs;
    keypairs["key1"] = QString("\"Hello\"");
    keypairs["key2"] = QString("\" world\"");
    qDebug() << "MSET:" << redis->mset(keypairs);
    qDebug() << "MGET:" << redis->mget("key1 key2 nonexisting");

    // Incr, incrby decr, decrby example.
    qDebug() << "SET:" << redis->set("count", "10");
    qDebug() << "INCR:" << redis->incr("count");
    qDebug() << "GET:" << redis->get("count");

    qDebug() << "INCRBY:" << redis->incrby("count",5);
    qDebug() << "GET:" << redis->get("count");

    qDebug() << "DECR:" << redis->decr("count");
    qDebug() << "GET:" << redis->get("count");

    qDebug() << "DECRBY:" << redis->decrby("count",5);
    qDebug() << "GET:" << redis->get("count");

    // SET and GET Range examples
    qDebug() << "SETRANGE:" << redis->setrange("key",6 ,"Redis");
    qDebug() << "GET:" << redis->get("key");

    qDebug() << "GETRANGE:" << redis->getrange("key",-5 ,-1);
    qDebug() << "GETRANGE:" << redis->getrange("key",0 ,4);
    qDebug() << "GETRANGE:" << redis->getrange("key",0 ,-1);


    //Hashmap example
    qDebug() << "HSET:" << redis->hset("hashmap","key1" ,"value1");
    qDebug() << "HSET:" << redis->hset("hashmap","key2" ,"value2");
    qDebug() << "HSET:" << redis->hset("hashmap","key3" ,"value3");
    qDebug() << "HGETALL:";

    QMap<QString,QVariant> hashmap = redis->hgetall("hashmap");
    QMapIterator<QString, QVariant> mi(hashmap);
    while (mi.hasNext())
    {
        mi.next();
        qDebug() << mi.key() << "=" << mi.value().toString();
    }


    // Raw Command example
    reply = redis->command("GET key");
    qDebug() << "RAW:" << "("<< reply.type << ")" << reply.value.toString();

    redis->subscribe("notifications");

    redis->psubscribe("news.*");

    //reply = redis->command("SUBSCRIBE notifications");
    //qDebug() << "("<< reply.type << ")" << reply.value.toStringList();

}
